#include <gtest/gtest.h>
#include "include/tracker.h"

TEST(TrackerBasicTest, Test1) { EXPECT_EQ(TrackerTest(), 1); }

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
