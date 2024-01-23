#include "context.h"

// 定义在tracker命名空间下的Context类
namespace tracker
{
    // Context类的开始方法
    void Context::Start()
    {
        // 如果已经开始，则直接返回
        if (is_start_.load())
        {
            return;
        }
        // 设置开始标志为true
        is_start_.store(true);

        // 创建并启动主线程
        main_thread_ = std::make_unique<std::thread>([this]
        {
            // 循环运行，直到停止标志为true
            while (true)
            {
                // 如果停止标志为true，跳出循环
                if (is_stop_)
                {
                    break;
                }
                // 运行主上下文
                main_context_.run();
            }
        });

        // 创建并启动报告线程
        report_thread_ = std::make_unique<std::thread>([this]
        {
            // 循环运行，直到停止标志为true
            while (true)
            {
                // 如果停止标志为true，跳出循环
                if (is_stop_)
                {
                    break;
                }
                // 运行报告上下文
                report_context_.run();
            }
        });
    }

    // Context类的析构函数
    Context::~Context()
    {
        // 设置停止标志为true
        is_stop_ = true;
        // 如果主线程存在，等待主线程结束
        if (main_thread_)
        {
            main_thread_->join();
        }
        // 如果报告线程存在，等待报告线程结束
        if (report_thread_)
        {
            report_thread_->join();
        }
    }
}
