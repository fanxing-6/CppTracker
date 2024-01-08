include_guard(GLOBAL)


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
)

set(MODULE_NAME "Tracker")

message(STATUS "libs ${LIBS}")



if (NOT spdlog_FOUND)
    message(WARNING "spdlog not found!")
else ()
    message(STATUS "Found spdlog on: " ${spdlog_DIR})
endif (NOT spdlog_FOUND)

add_library(${MODULE_NAME} SHARED ${TRACKER_SRCS})
target_link_libraries(${MODULE_NAME} PRIVATE ${LIBS} spdlog::spdlog sqlite_orm::sqlite_orm unofficial::sqlite3::sqlite3 cryptopp::cryptopp)