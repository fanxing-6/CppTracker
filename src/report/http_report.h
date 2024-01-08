#pragma once

#include <memory>
#include <string>

namespace spdlog
{
    class logger;
}

namespace tracker
{
    class HttpReporter
    {
    public:
        explicit HttpReporter(std::shared_ptr<spdlog::logger> logger);
        ~HttpReporter() = default;
        HttpReporter& Host(const std::string& host);
        HttpReporter& Port(const std::string& port);
        HttpReporter& Topic(const std::string& topic);
        HttpReporter& Body(const std::string& body);
        bool Report();

    private:
        std::string host_;
        std::string topic_;
        std::string port_;
        std::string body_;
        std::shared_ptr<spdlog::logger> logger_;
    };
}
