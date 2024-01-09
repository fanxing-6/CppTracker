include_guard(GLOBAL)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tracker_config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/tracker_config.h)

add_definitions(-D_WIN32_WINNT=0x0601)
add_definitions(-DBOOST_JSON_NO_LIB)
add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)


set(TRACKER_SRCS
        tracker.cpp
        tracker_core.cpp
        database/database.cpp
        report/http_report.cpp
        report/tracker_report.cpp
        crypt/crypt.cpp
        common/common_service.cpp
)

set(MODULE_NAME "Tracker")

message(STATUS "libs ${LIBS}")



if (NOT spdlog_FOUND)
    message(WARNING "spdlog not found!")
else ()
    message(STATUS "Found spdlog on: " ${spdlog_DIR})
endif (NOT spdlog_FOUND)

add_library(${MODULE_NAME} SHARED ${TRACKER_SRCS})
target_link_libraries(${MODULE_NAME} PRIVATE ${LIBS} spdlog::spdlog sqlite_orm::sqlite_orm
        unofficial::sqlite3::sqlite3 cryptopp::cryptopp nlohmann_json::nlohmann_json)