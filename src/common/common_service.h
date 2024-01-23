#pragma once

#include <string>

#include "nlohmann/json.hpp"

// 定义在tracker命名空间下的CommonService结构体
namespace tracker
{
    struct CommonService
    {
    public:
        // 服务器主机名
        std::string host;
        // 服务器端口号
        std::string port;
        // 主题
        std::string topic;
        // 用户ID
        std::string user_id;
        // 应用版本
        std::string app_version;
        // 应用名称
        std::string app_name;

        // 自定义数据，使用nlohmann::json库进行json格式的处理
        nlohmann::json custom_data;

        // 系统版本
        std::string system_version;
        // 设备名称
        std::string device_name;
        // 设备ID
        std::string device_id;
        // 跟踪器版本
        std::string tracker_version;
        // 生命周期ID
        std::string lifecycle_id;

    public:
        // CommonService的构造函数
        CommonService();

        // 获取进程时间的静态方法
        static std::string GetProcessTime();
        // 获取当前日期的静态方法
        static std::string GetNowDate();
        // 生成随机ID的静态方法
        static std::string GetRandomId();

    private:
        // 初始化CommonService的私有方法
        void Init();
    };
}
