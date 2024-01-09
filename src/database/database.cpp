#include "database.h"
#include "sqlite_orm/sqlite_orm.h"
#include <utility>


namespace tracker
{
    using namespace sqlite_orm;

    inline auto InitStorge(const std::string& path)
    {
        return make_storage(path, make_table("tracker_data",
                                             make_column("id", &TrackerDb::Data::id, primary_key().autoincrement()),
                                             make_column("priority", &TrackerDb::Data::priority),
                                             make_column("timestamp", &TrackerDb::Data::timestamp),
                                             make_column("content", &TrackerDb::Data::content)));
    }

    class TrackerDbImpl
    {
    public:
        using DBStorage = decltype(InitStorge(""));

    public:
        explicit TrackerDbImpl(std::string db_path): db_path_(std::move(db_path))
        {
            storage_ = std::make_unique<DBStorage>(InitStorge(db_path_));
            storage_->sync_schema();
        }

        ~TrackerDbImpl() = default;

        void InsertData(const TrackerDb::Data& data)
        {
            auto guard = storage_->transaction_guard();
            storage_->insert(data);
            guard.commit();
        }

        void DeleteDatas(const std::vector<TrackerDb::Data>& datas)
        {
            auto guard = storage_->transaction_guard();
            for (const auto& data : datas)
            {
                storage_->remove_all<TrackerDb::Data>(where(c(&TrackerDb::Data::id) == data.id));
            }
            guard.commit();
        }

        void DeleteData(const TrackerDb::Data& data)
        {
            auto guard = storage_->transaction_guard();
            storage_->remove_all<TrackerDb::Data>(where(c(&TrackerDb::Data::id) == data.id));
            guard.commit();
        }

        std::vector<TrackerDb::Data> QueryData(int32_t limit_size)
        {
            auto limited = storage_->get_all<TrackerDb::Data>(order_by(&TrackerDb::Data::priority).desc(),
                                                              limit(limit_size));
            return limited;
        }

    private:
        std::string db_path_;
        std::unique_ptr<DBStorage> storage_;
    };

    TrackerDb::TrackerDb(std::string db_path): impl_(std::make_unique<TrackerDbImpl>(std::move(db_path)))
    {
    }

    TrackerDb::~TrackerDb() = default;

    void TrackerDb::InsertData(const Data& data)
    {
        impl_->InsertData(data);
    }

    void TrackerDb::DeleteData(const Data& data)
    {
        impl_->DeleteData(data);
    }

    void TrackerDb::DeleteDatas(const std::vector<Data>& datas)
    {
        impl_->DeleteDatas(datas);
    }

    std::vector<TrackerDb::Data> TrackerDb::QueryData(int32_t limit)
    {
        return impl_->QueryData(limit);
    }
}
