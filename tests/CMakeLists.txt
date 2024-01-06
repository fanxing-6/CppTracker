cmake_minimum_required(VERSION 3.27)
project(TrackerTest)

set(TEST_SRC test.cpp)



find_package(GTest CONFIG REQUIRED)



add_executable(tracker_test ${TEST_SRC})

# ȷ�����ӵ�GoogleTest�Ŀ�
target_link_libraries(tracker_test Tracker GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main spdlog::spdlog)

# ���Ӳ���
add_test(NAME TrackerTests COMMAND tracker_test)