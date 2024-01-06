#pragma once

#include <cstdint>

#define BURIED_EXPORT __declspec(dllexport)


extern "C" {
BURIED_EXPORT int TrackerTest();

typedef struct Tracker Tracker;

struct TrackerConfig
{
	const char* host;
	const char* topic;
	const char* user_id;
	const char* version;
	const char* custom_data;
};

BURIED_EXPORT Tracker* Tracker_Create(const char* work_dir);
BURIED_EXPORT void Tracker_Destroy(Tracker* tracker);
BURIED_EXPORT int32_t Tracker_Start(Tracker* tracker, const TrackerConfig* config);
BURIED_EXPORT int32_t Tracker_Report(Tracker* tracker, const char* report_data, uint32_t priority);
}
