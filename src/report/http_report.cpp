#include "boost/asio/connect.hpp"
#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "spdlog/spdlog.h"

#include "http_report.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

namespace tracker
{
    static boost::asio::io_context ioc;

    HttpReporter::HttpReporter(std::shared_ptr<spdlog::logger> logger) : logger_(logger)
    {
    }

    HttpReporter& HttpReporter::Host(const std::string& host)
    {
        host_ = host;
        return *this;
    }

    HttpReporter& HttpReporter::Port(const std::string& port)
    {
        port_ = port;
        return *this;
    }

    HttpReporter& HttpReporter::Topic(const std::string& topic)
    {
        topic_ = topic;
        return *this;
    }

    HttpReporter& HttpReporter::Body(const std::string& body)
    {
        body_ = body;
        return *this;
    }

    bool HttpReporter::Report()
    {
        try
        {
            // 设置HTTP版本为11，即HTTP/1.1
            int version = 11;

            // 初始化TCP解析器，用于DNS解析
            tcp::resolver resolver(ioc);

            // 创建一个TCP流，用于网络通信
            beast::tcp_stream stream(ioc);

            // 设置DNS查询，包括主机名和端口
            boost::asio::ip::tcp::resolver::query query(host_, port_);

            // 解析DNS，获取IP地址
            auto const results = resolver.resolve(query);

            // 使用获取的IP地址建立TCP连接
            stream.connect(results);

            // 构建HTTP POST请求，设置请求路径和HTTP版本
            http::request<http::string_body> req{http::verb::post, topic_, version};

            // 设置HTTP请求头部的Host字段
            req.set(http::field::host, host_);

            // 设置HTTP请求头部的User-Agent字段
            req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

            // 设置HTTP请求头部的Content-Type字段为application/json
            req.set(http::field::content_type, "application/json");

            // 设置HTTP请求体为JSON格式
            req.body() = body_;

            // 准备发送的HTTP请求数据
            req.prepare_payload();

            // 向服务器发送HTTP请求
            http::write(stream, req);

            // 创建一个buffer用于接收HTTP响应
            beast::flat_buffer buffer;

            // 用于存储HTTP响应的对象
            http::response<http::dynamic_body> res;

            // 从服务器读取HTTP响应
            http::read(stream, buffer, res);

            // 初始化错误码，用于处理网络错误
            beast::error_code ec;

            // 优雅地关闭TCP连接
            stream.socket().shutdown(tcp::socket::shutdown_both, ec);

            // 检查是否有错误，如果有且不是“未连接”错误，则抛出异常
            if (ec && ec != beast::errc::not_connected) throw beast::system_error{ec};

            // 获取HTTP响应状态码
            auto res_status = res.result();

            // 检查HTTP响应状态码是否为200 OK
            if (res_status != http::status::ok)
            {
                // 如果不是200 OK，记录错误并返回false
                SPDLOG_LOGGER_ERROR(logger_,
                                    "report error " + std::to_string(res.result_int()));
                return false;
            }

            // 将HTTP响应体转换为字符串
            std::string res_body = boost::beast::buffers_to_string(res.body().data());

            // 记录成功的HTTP响应
            SPDLOG_LOGGER_TRACE(logger_, "report success" + res_body);
        }
        catch (std::exception const& e)
        {
            // 捕获并记录异常，返回false
            SPDLOG_LOGGER_ERROR(logger_, "report error " + std::string(e.what()));
            return false;
        }
        // 如果一切正常，返回true
        return true;
    }
}
