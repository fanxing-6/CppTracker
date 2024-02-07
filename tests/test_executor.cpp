#include <chrono>
#include <iostream>
#include <thread>
#include <format>

#include "gtest/gtest.h"
#include "boost/asio.hpp"

std::string thread_id_to_string(const std::thread::id& id) {
    std::ostringstream oss;
    oss << id;
    return oss.str();
}

TEST(ExecutorTest, Test1)
{
    boost::asio::io_context io_context1;
    boost::asio::io_context io_context2;
    boost::asio::io_context io_context3;

    boost::asio::io_context::strand strand1(io_context1);
    boost::asio::io_context::strand strand2(io_context2);
    boost::asio::io_context::strand strand3(io_context3);

    boost::asio::post(strand1, []()
    {
        std::cout << std::format("Operation 1 executed in strand1 , thread id = {}\n", thread_id_to_string(std::this_thread::get_id()));
    });

    boost::asio::post(strand2, [&]()
    {
        std::cout << std::format("Operation 2 executed in strand2 , thread id = {}\n", thread_id_to_string(std::this_thread::get_id()));

        boost::asio::post(strand2, []()
        {
            std::cout << std::format("Operation 3 executed in strand2 , thread id = {}\n", thread_id_to_string(std::this_thread::get_id()));
        });

        boost::asio::post(strand1, []()
        {
            std::cout << std::format("Operation 4 executed in strand1 , thread id = {}\n", thread_id_to_string(std::this_thread::get_id()));
        });

        boost::asio::post(strand3, []()
        {
            std::cout << std::format("Operation 5 executed in strand3 , thread id = {}\n", thread_id_to_string(std::this_thread::get_id()));
        });
    });

    boost::asio::post(strand3, []()
    {
        std::cout << std::format("Operation 6 executed in strand3 , thread id = {}\n", thread_id_to_string(std::this_thread::get_id()));
    });

    std::thread io_thread([&]()
    {
        io_context1.run();
        io_context2.run();

        boost::asio::post(strand3, []()
        {
            std::cout << std::format("Operation 7 executed in strand3 , thread id = {}\n", thread_id_to_string(std::this_thread::get_id()));
        });

        for (int i = 0; i < 20; ++i)
        {
            io_context1.run();
            io_context2.run();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    boost::asio::post(strand3, []()
    {
        std::cout << std::format("Operation 8 executed in strand3 , thread id = {}\n", thread_id_to_string(std::this_thread::get_id()));
    });

    io_context3.run();

    std::cout << "Main thread id = " << thread_id_to_string(std::this_thread::get_id()) << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    boost::asio::post(strand3, []()
    {
        std::cout << std::format("Operation 9 executed in strand3 , thread id = {}\n", thread_id_to_string(std::this_thread::get_id()));
    });

    for (int i = 0; i < 20; ++i)
    {
        io_context3.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    io_thread.join();
}
