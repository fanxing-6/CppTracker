#pragma once

#include <boost/asio/io_context.hpp>
#include "boost/asio/io_context_strand.hpp"
#include <boost/beast/core/error.hpp>


namespace tracker
{
    class Context
    {
    public:
        static Context& GetGlobalContext()
        {
            static Context global_context;
            return global_context;
        }

        ~Context();

        using Strand = boost::asio::io_context::strand;
        using IOContext = boost::asio::io_context;

        Strand& GetMainStrand() { return main_strand_; }

        Strand& GetReportStrand() { return report_strand_; }

        IOContext& GetMainContext() { return main_context_; }

        void Start();

    public:
        Context(const Context&) = delete;
        Context& operator=(const Context&) = delete;

    private:
        Context(): main_strand_(main_context_), report_strand_(report_context_)
        {
        }

    private:
        boost::asio::io_context main_context_;
        boost::asio::io_context report_context_;

        boost::asio::io_context::strand main_strand_;
        boost::asio::io_context::strand report_strand_;

        std::unique_ptr<std::thread> main_thread_;
        std::unique_ptr<std::thread> report_thread_;

        std::atomic<bool> is_start_{false};
        std::atomic<bool> is_stop_{false};
    };
}
