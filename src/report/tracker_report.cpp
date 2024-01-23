#include "boost/asio/deadline_timer.hpp"
#include "tracker_report.h"
#include <src/crypt/crypt.h>
#include <src/database/database.h>

#include <utility>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <src/context/context.h>

#include "http_report.h"

namespace tracker
{
    static const std::string DbName = "tracker.db";

    class TrackerReportImpl
    {
    public:
        TrackerReportImpl(std::shared_ptr<spdlog::logger> logger, CommonService common_service, std::string work_path);
        ~TrackerReportImpl();
        void Start();
        void InsertData(const TrackerData& data);

    private:
        void Init_();
        void ReportCache_();
        void NextCycle_();
        TrackerDb::Data MakeDbData_(const TrackerData& data);
        std::string GenReportData_(const std::vector<TrackerDb::Data>& datas);
        bool ReportData_(const std::string& data);


        std::shared_ptr<spdlog::logger> logger_;
        CommonService common_service_;
        std::string work_dir_;
        std::unique_ptr<TrackerDb> db_;
        std::unique_ptr<tracker::Crypt> crypt_;
        std::unique_ptr<boost::asio::deadline_timer> timer_;
        std::vector<TrackerDb::Data> data_caches_;
    };

    /* TrackerReport */

    TrackerReport::TrackerReport(const std::shared_ptr<spdlog::logger>& logger, const CommonService& common_service,
                                 const std::string& work_path)
        : impl_(std::make_unique<TrackerReportImpl>(logger, common_service, work_path))
    {
    }

    TrackerReport::~TrackerReport() = default;

    void TrackerReport::Start()
    {
        impl_->Start();
    }

    void TrackerReport::InsertData(const TrackerData& data)
    {
        impl_->InsertData(data);
    }

    /* TrackerReportImpl */

    TrackerReportImpl::TrackerReportImpl(std::shared_ptr<spdlog::logger> logger, CommonService common_service,
                                         std::string work_path)
        : logger_(std::move(logger)), common_service_(std::move(common_service)), work_dir_(std::move(work_path))
    {
        if (logger_ == nullptr)
        {
            logger_ = spdlog::stdout_color_mt("tracker");
        }
        std::string key = AEScrypt::GetKey("tracker_salt", "tracker_password");
        crypt_ = std::make_unique<AEScrypt>(key);
        SPDLOG_LOGGER_INFO(logger_, "tracker report init success");
        Context::GetGlobalContext().GetReportStrand().post([this] { Init_(); });
    }

    TrackerReportImpl::~TrackerReportImpl() = default;

    void TrackerReportImpl::Start()
    {
        SPDLOG_LOGGER_INFO(logger_, "TrackerReportImpl::Start");

        timer_ = std::make_unique<boost::asio::deadline_timer>(Context::GetGlobalContext().GetMainContext(),
                                                               boost::posix_time::seconds(5));
        timer_->async_wait(Context::GetGlobalContext().GetReportStrand().wrap(
            [this](const boost::system::error_code& ec)
            {
                if (ec)
                {
                    logger_->error("TrackerReportImpl::Start error: {}", ec.message());
                    return;
                }
                ReportCache_();
            }));
    }

    void TrackerReportImpl::InsertData(const TrackerData& data)
    {
        Context::GetGlobalContext().GetReportStrand().post(
            [this,data] { db_->InsertData(MakeDbData_(data)); }
        );
    }

    void TrackerReportImpl::Init_()
    {
        std::filesystem::path db_path = work_dir_;
        SPDLOG_LOGGER_INFO(logger_, "TrackerReportImpl::Init_ db_path: {}", db_path.string());
        db_path /= DbName;
        db_ = std::make_unique<TrackerDb>(db_path.string());
    }

    void TrackerReportImpl::ReportCache_()
    {
        SPDLOG_LOGGER_INFO(logger_, "TrackerReportImpl::ReportCache_");
        if (data_caches_.empty())
        {
            data_caches_ = db_->QueryData(10);
        }

        if (!data_caches_.empty())
        {
            std::string report_data = GenReportData_(data_caches_);
            if (ReportData_(report_data))
            {
                db_->DeleteDatas(data_caches_);
                data_caches_.clear();
            }
        }

        NextCycle_();
    }

    void TrackerReportImpl::NextCycle_()
    {
        SPDLOG_LOGGER_INFO(logger_, "TrackerReportImpl::NextCycle_");
        timer_->expires_at(timer_->expires_at() + boost::posix_time::seconds(5));
        timer_->async_wait([this](const boost::system::error_code& ec)
        {
            if (ec)
            {
                logger_->error("TrackerReportImpl::NextCycle_ error: {}", ec.message());
                return;
            }
            Context::GetGlobalContext().GetReportStrand().post([this] { ReportCache_(); });
        });
    }

    TrackerDb::Data TrackerReportImpl::MakeDbData_(const TrackerData& data)
    {
        TrackerDb::Data db_data;
        db_data.id = -1;
        db_data.priority = data.priority;
        db_data.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
            .count();
        nlohmann::json json_data;
        json_data["title"] = data.title;
        json_data["data"] = data.data;
        json_data["user_id"] = common_service_.user_id;
        json_data["app_version"] = common_service_.app_version;
        json_data["app_name"] = common_service_.app_name;
        json_data["custom_data"] = common_service_.custom_data;
        json_data["system_version"] = common_service_.system_version;
        json_data["device_name"] = common_service_.device_name;
        json_data["device_id"] = common_service_.device_id;
        json_data["buried_version"] = common_service_.tracker_version;
        json_data["lifecycle_id"] = common_service_.lifecycle_id;
        json_data["priority"] = data.priority;
        json_data["timestamp"] = CommonService::GetNowDate();
        json_data["process_time"] = CommonService::GetProcessTime();
        json_data["report_id"] = CommonService::GetRandomId();
        std::string report_data = crypt_->Encrypt(json_data.dump());
        db_data.content = std::vector<char>(report_data.begin(), report_data.end());
        SPDLOG_LOGGER_INFO(logger_, "TrackerReportImpl::MakeDbData_ content size: {}", db_data.content.size());
        return db_data;
    }

    std::string TrackerReportImpl::GenReportData_(const std::vector<TrackerDb::Data>& datas)
    {
        nlohmann::json json_datas;
        for (const auto& data : datas)
        {
            std::string content = crypt_->Decrypt(data.content.data(), data.content.size());
            SPDLOG_LOGGER_INFO(logger_, "TrackerReportImpl::GenReportData_ content size: {}", content.size());
            json_datas.push_back(content);
        }
        std::string ret = json_datas.dump();
        return ret;
    }

    bool TrackerReportImpl::ReportData_(const std::string& data)
    {
        HttpReporter reporter(logger_);
        return reporter.Host(common_service_.host)
                       .Topic(common_service_.topic)
                       .Port(common_service_.port)
                       .Body(data)
                       .Report();
    }
}
