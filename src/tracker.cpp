#include <iostream>
#include "include/tracker.h"
#include "tracker_core.h"


extern "C" {
int TrackerTest()
{
    std::cout << "Test" << std::endl;
    return 1;
}

Tracker* Tracker_Create(const char* work_dir)
{
    if (!work_dir)
    {
        return nullptr;
    }
    return new Tracker(work_dir);
}
}

void Tracker_Destroy(Tracker* tracker)
{
    if (tracker)
    {
        delete tracker;
    }
}

int32_t Tracker_Start(Tracker* tracker, const TrackerConfig* config)
{
    if (!tracker || !config)
    {
        return TrackerResult::kTrackerInvalidParam;
    }
    Tracker::Config tracker_config;

    if (config->host)
    {
        tracker_config.host = config->host;
    }
    if (config->port)
    {
        tracker_config.port = config->port;
    }
    if (config->topic)
    {
        tracker_config.topic = config->topic;
    }
    if (config->user_id)
    {
        tracker_config.user_id = config->user_id;
    }
    if (config->app_version)
    {
        tracker_config.app_version = config->app_version;
    }
    if (config->app_name)
    {
        tracker_config.app_name = config->app_name;
    }
    if (config->custom_data)
    {
        tracker_config.custom_data = config->custom_data;
    }
    return tracker->Start(tracker_config);
}

int32_t Tracker_Report(Tracker* tracker, const char* title, const char* report_data, uint32_t priority)
{
    if (!tracker || !report_data)
    {
        return TrackerResult::kTrackerInvalidParam;
    }
    return tracker->Report(title, report_data, priority);
}
