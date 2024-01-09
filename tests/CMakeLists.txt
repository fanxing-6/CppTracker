cmake_minimum_required(VERSION 3.27)
project(TrackerTest)

set(TEST_SRC
        test_db.cpp
        test.cpp
        test_http.cpp
        test_crypt.cpp
        ../src/tracker.cpp
        ../src/report/http_report.cpp
        ../src/database/database.cpp
        ../src/tracker_core.cpp
        ../src/crypt/crypt.cpp
        ../src/common/common_service.cpp
        test_common_service.cpp
)

# 查找并引入spdlog库
find_package(GTest CONFIG REQUIRED)

add_executable(tracker_test ${TEST_SRC})

# 确保链接到GoogleTest的库
target_link_libraries(tracker_test Tracker GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main spdlog::spdlog
        sqlite_orm::sqlite_orm unofficial::sqlite3::sqlite3 cryptopp::cryptopp nlohmann_json::nlohmann_json)

# 添加测试
add_test(NAME TrackerTests COMMAND tracker_test)

