#include "include/tracker.h"
#include <gtest/gtest.h>


TEST(TrackerBasicTest, Test1) { EXPECT_EQ(TrackerTest(), 1); }
TEST(TrackerBasicTest, Test2) { Tracker_Create(R"(D:\AAAMyCode\Clion\CppTracker\cmake-build-debug\TrackerPoint)"); }


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
