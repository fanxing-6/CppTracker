#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include "spdlog/logger.h"

#include "tracker_common.h"
#include "include/tracker.h"
#include "report/tracker_report.h"

namespace spdlog
{
    class spdlog;
}


struct Tracker
{
public:
    struct Config
    {
        std::string host;
        std::string port;
        std::string topic;
        std::string user_id;
        std::string app_version;
        std::string app_name;
        std::string custom_data;
    };

public:
    explicit Tracker(const std::string& work_dir);
    ~Tracker();

    TrackerResult Start(const Config& config);
    TrackerResult Report(std::string title, const std::string& report_data, uint32_t priority);

public:
    std::shared_ptr<spdlog::logger> Logger();

private:
    void InitWorkPath_(const std::string& work_dir);
    void InitLogger_();

private:
    std::shared_ptr<spdlog::logger> logger_;
    std::unique_ptr<tracker::TrackerReport> tracker_report_;
    std::filesystem::path work_path_;
};
