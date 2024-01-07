include_guard(GLOBAL)

set(TRACKER_SRCS
        tracker.cpp
        tracker_core.cpp
        database/database.cpp
)

set(MODULE_NAME "Tracker")

message(STATUS "libs ${LIBS}")

add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

if (NOT spdlog_FOUND)
    message(WARNING "spdlog not found!")
else ()
    message(STATUS "Found spdlog on: " ${spdlog_DIR})
endif (NOT spdlog_FOUND)

add_library(${MODULE_NAME} SHARED ${TRACKER_SRCS})
target_link_libraries(${MODULE_NAME} PRIVATE ${LIBS} spdlog::spdlog sqlite_orm::sqlite_orm unofficial::sqlite3::sqlite3)
