cmake_minimum_required(VERSION 3.27)
project(TrackerTest)

set(TEST_SRC
        test_db.cpp
        test.cpp
        test_http.cpp
        ../src/tracker.cpp
        ../src/report/http_report.cpp
        ../src/database/database.cpp
        ../src/tracker_core.cpp
)

# 查找并引入spdlog库
find_package(GTest CONFIG REQUIRED)

add_executable(tracker_test ${TEST_SRC})

# 确保链接到GoogleTest的库
target_link_libraries(tracker_test Tracker GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main spdlog::spdlog
        sqlite_orm::sqlite_orm unofficial::sqlite3::sqlite3)

# 首先复制 Tracker.dll
add_custom_command(TARGET tracker_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Tracker> $<TARGET_FILE_DIR:tracker_test>)

# 然后复制 spdlogd.dll
add_custom_command(TARGET tracker_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE_DIR:Tracker>/spdlogd.dll $<TARGET_FILE_DIR:tracker_test>)

add_custom_command(TARGET tracker_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE_DIR:Tracker>/fmtd.dll $<TARGET_FILE_DIR:tracker_test>)

add_custom_command(TARGET tracker_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE_DIR:Tracker>/sqlite3.dll $<TARGET_FILE_DIR:tracker_test>)

# 添加测试
add_test(NAME TrackerTests COMMAND tracker_test)

