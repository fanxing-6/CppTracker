#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "tracker_core.h"

#include "common/common_service.h"
#include "context/context.h"


Tracker::Tracker(const std::string& work_dir)
{
    tracker::Context::GetGlobalContext().Start();
    InitWorkPath_(work_dir);
    InitLogger_();

    SPDLOG_LOGGER_INFO(Logger(), "Tracker init success.");
}

Tracker::~Tracker() = default;

TrackerResult Tracker::Start(const Config& config)
{
    tracker::CommonService common_service;
    common_service.host = config.host;
    common_service.port = config.port;
    common_service.topic = config.topic;
    common_service.user_id = config.user_id;
    common_service.app_version = config.app_version;
    common_service.app_name = config.app_name;
    common_service.custom_data = nlohmann::json::parse(config.custom_data);

    tracker_report_ = std::make_unique<tracker::TrackerReport>(
        logger_, std::move(common_service), work_path_.string());
    tracker_report_->Start();
    return TrackerResult::kTrackerOk;
}

TrackerResult Tracker::Report(std::string title, const std::string& report_data, uint32_t priority)
{
    tracker::TrackerData tracker_data;
    tracker_data.title = std::move(title);
    tracker_data.data = report_data;
    tracker_data.priority = priority;
    tracker_report_->InsertData(tracker_data);

    return TrackerResult::kTrackerOk;
}

std::shared_ptr<spdlog::logger> Tracker::Logger()
{
    return logger_;
}

void Tracker::InitWorkPath_(const std::string& work_dir)
{
    const std::filesystem::path _work_dir(work_dir);
    if (!std::filesystem::exists(_work_dir))
    {
        std::filesystem::create_directories(_work_dir);
    }

    work_path_ = _work_dir / "tracker";
    if (!std::filesystem::exists(work_path_))
    {
        std::filesystem::create_directories(work_path_);
    }
}

void Tracker::InitLogger_()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    const std::filesystem::path _log_dir = work_path_ / "tracker.log";
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(_log_dir.string(), true);

    logger_ = std::make_shared<spdlog::logger>("tracker_sink", spdlog::sinks_init_list{console_sink, file_sink});
    logger_->set_pattern("[%c] [%s:%#] [%l] %v");
    logger_->set_level(spdlog::level::trace);
}
