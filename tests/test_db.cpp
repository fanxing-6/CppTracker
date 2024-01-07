#include <filesystem>
#include "gtest/gtest.h"
#include "src/database/database.h"

TEST(DatabaseTest, Test1)
{
    std::filesystem::path db_path =
        R"(D:\AAAMyCode\Clion\CppTracker\cmake-build-debug\TrackerPoint\tracker\tracker.db)";
    if (std::filesystem::exists(db_path))
    {
        std::filesystem::remove(db_path);
    }

    tracker::TrackerDb db(db_path.string());
    {
        tracker::TrackerDb::Data data{
            -1, 1, 2, std::vector<char>{'h', 'e', 'l', 'l', 'o'}
        };
        db.InsertData(data);
    }
    auto datas = db.QueryData(10);
    EXPECT_EQ(datas.size(), 1);

    {
        tracker::TrackerDb::Data data{
            -1, 2, 2, std::vector<char>{'h', 'e', 'l', 'l', 'o'}
        };
        db.InsertData(data);
    }
    datas = db.QueryData(10);
    EXPECT_EQ(datas.size(), 2);

    {
        tracker::TrackerDb::Data data{
            -1, 3, 2, std::vector<char>{'h', 'e', 'l', 'l', 'o'}
        };
        db.InsertData(data);
    }
    datas = db.QueryData(10);
    EXPECT_EQ(datas.size(), 3);

    {
        tracker::TrackerDb::Data data{
            -1, 4, 2, std::vector<char>{'h', 'e', 'l', 'l', 'o'}
        };
        db.InsertData(data);
    }
    datas = db.QueryData(10);
    EXPECT_EQ(datas.size(), 4);

    {
        tracker::TrackerDb::Data data{
            -1, 5, 2, std::vector<char>{'h', 'e', 'l', 'l', 'o'}
        };
        db.InsertData(data);
    }
    datas = db.QueryData(10);
    EXPECT_EQ(datas.size(), 5);

    EXPECT_EQ(datas[0].priority, 5);
    EXPECT_EQ(datas[1].priority, 4);
    EXPECT_EQ(datas[2].priority, 3);
    EXPECT_EQ(datas[3].priority, 2);
    EXPECT_EQ(datas[4].priority, 1);

    db.DeleteData(datas[0]);
    datas = db.QueryData(10);
    EXPECT_EQ(datas.size(), 4);

    for (int i = 0; i < 100; ++i)
    {
        tracker::TrackerDb::Data data{
            -1, i, 1, std::vector<char>{'h', 'e', 'l', 'l', 'o'}
        };
        db.InsertData(data);
    }

    datas = db.QueryData(10);

    EXPECT_EQ(datas.size(), 10);

    std::filesystem::remove(db_path);
}