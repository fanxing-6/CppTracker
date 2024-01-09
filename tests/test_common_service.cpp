#include <gtest/gtest.h>
#include "src/common/common_service.h"

TEST(CommonServiceTest, Test1)
{
    tracker::CommonService common_service;

    EXPECT_TRUE(!common_service.system_version.empty());
    EXPECT_TRUE(!common_service.device_name.empty());
    EXPECT_TRUE(!common_service.device_id.empty());
    EXPECT_TRUE(!common_service.lifecycle_id.empty());
}
