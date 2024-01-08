#include <iostream>
#include <string>
#include <src/tracker_core.h>
#include <src/report/http_report.h>

#include "gtest/gtest.h"
#include "boost/asio/connect.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

TEST(HttpTest, TestBeast)
{
    try
    {
        auto const host = "www.baidu.com"; // 目标主机名
        auto const port = "80"; // 使用的端口号，HTTP 默认端口是 80
        auto const target = "/"; // 请求的目标路径，这里是根路径
        int version = 11; // HTTP 版本，这里使用的是 HTTP/1.1

        net::io_context ioc; // I/O 上下文，用于管理 I/O 服务
        tcp::resolver resolver(ioc); // 域名解析器，用于将主机名和端口解析为 TCP 端点
        beast::tcp_stream stream(ioc); // TCP 流，用于网络通信

        auto const results = resolver.resolve(host, port); // 解析域名和端口
        stream.connect(results); // 使用解析得到的端点连接到服务器

        // 构建 HTTP GET 请求
        http::request<http::string_body> req{http::verb::get, target, version};
        req.set(http::field::host, host); // 设置请求头中的 Host 字段
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING); // 设置 User-Agent 字段

        http::write(stream, req); // 向服务器发送 HTTP 请求

        beast::flat_buffer buffer; // 用于存储响应的缓冲区
        http::response<http::dynamic_body> res; // 存储 HTTP 响应
        http::read(stream, buffer, res); // 从服务器读取响应

        std::cout << res << std::endl; // 打印响应

        // 关闭连接
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec); // 关闭 TCP 流
        if (ec && ec != beast::errc::not_connected)
        {
            throw beast::system_error{ec}; // 如果发生错误且不是“未连接”错误，则抛出异常
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl; // 捕获并打印异常信息
    }
}


TEST(HttpTest, TestHttpReport)
{
    auto logger = Tracker(R"(D:\AAAMyCode\Clion\CppTracker\cmake-build-debug\TrackerPoint\TestHttpReport)").Logger();

    tracker::HttpReporter reporter(logger);
    reporter.Host("www.baidu.com").Port("8080").Topic("").Body("").Report();
}
