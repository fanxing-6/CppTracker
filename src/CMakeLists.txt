include_guard(GLOBAL)

set(TRACKER_SRCS
        tracker.cpp)

set(MODULE_NAME "Tracker")

message(STATUS "libs ${LIBS}")

if (BUILD_TRACKER_SHARED_LIBS)
    add_library(${MODULE_NAME} SHARED ${TRACKER_SRCS})
    target_link_libraries(${MODULE_NAME} PRIVATE ${LIBS})
endif ()

if (BUILD_TRACKER_STATIC_LIBS)
    add_library(${MODULE_NAME}_static STATIC ${TRACKER_SRCS})
    target_link_libraries(${MODULE_NAME} PRIVATE ${LIBS})
endif ()
