#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace tracker
{
    class TrackerDbImpl;

    class TrackerDb
    {
    public:
        struct Data
        {
            int32_t id;
            int32_t priority;
            uint64_t timestamp;
            std::vector<char> content;
        };

    public:
        explicit TrackerDb(std::string db_path);
        ~TrackerDb();
        void InsertData(const Data& data);
        void DeleteData(const Data& data);
        void DeleteDatas(const std::vector<Data>& datas);
        std::vector<Data> QueryData(int32_t limit);

    private:
        std::unique_ptr<TrackerDbImpl> impl_;
    };
}
