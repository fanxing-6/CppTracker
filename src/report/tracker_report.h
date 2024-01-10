#pragma once

#include "src/common/common_service.h"

namespace spdlog
{
    class logger;
}

namespace tracker
{
    struct TrackerData
    {
        std::string title;
        std::string data;
        uint32_t priority;
    };

    class TrackerReportImpl;

    class TrackerReport
    {
    public:
        TrackerReport(const std::shared_ptr<spdlog::logger>& logger, const CommonService& common_service, const std::string& work_path);
        ~TrackerReport();

        void Start();
        void InsertData(const TrackerData& data);

    private:
        std::unique_ptr<TrackerReportImpl> impl_;
    };
}
