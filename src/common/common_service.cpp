#include "common_service.h"

#include <random>

#include "windows.h"

#include "src/tracker_config.h"

namespace tracker
{
    // CommonService的构造函数，调用Init()函数进行初始化
    CommonService::CommonService() { Init(); }

    // 写入注册表的函数
    // key: 注册表键名
    // value: 注册表键值
    static void WriteRegister(const std::string& key, const std::string& value)
    {
        HKEY h_key;
        // 创建或打开一个注册表键
        LONG ret = ::RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Tracker", 0, NULL,
                                     REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
                                     &h_key, NULL);
        // 如果操作不成功，返回
        if (ret != ERROR_SUCCESS)
        {
            return;
        }
        // 设置注册表键的值
        ret = ::RegSetValueExA(h_key, key.c_str(), 0, REG_SZ,
                               reinterpret_cast<const BYTE*>(value.c_str()),
                               value.size());
        // 如果操作不成功，返回
        if (ret != ERROR_SUCCESS)
        {
            return;
        }
        // 关闭注册表键
        ::RegCloseKey(h_key);
    }

    // 读取注册表的函数
    // key: 注册表键名
    // 返回值: 注册表键值
    static std::string ReadRegister(const std::string& key)
    {
        HKEY h_key;
        // 打开一个注册表键
        LONG ret = ::RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Tracker", 0,
                                   KEY_ALL_ACCESS, &h_key);
        // 如果操作不成功，返回空字符串
        if (ret != ERROR_SUCCESS)
        {
            return "";
        }
        char buf[1024] = {0};
        DWORD buf_size = sizeof(buf);
        // 查询注册表键的值
        ret = ::RegQueryValueExA(h_key, key.c_str(), NULL, NULL,
                                 reinterpret_cast<BYTE*>(buf), &buf_size);
        // 如果操作不成功，返回空字符串
        if (ret != ERROR_SUCCESS)
        {
            return "";
        }
        // 关闭注册表键
        ::RegCloseKey(h_key);
        // 返回注册表键值
        return buf;
    }

    // 获取设备ID的函数
    static std::string GetDeviceId()
    {
        // 设备ID在注册表中的键名
        static constexpr auto kDeviceIdKey = "device_id";
        // 从注册表中读取设备ID
        static std::string device_id = ReadRegister(kDeviceIdKey);
        // 如果设备ID为空（即注册表中没有该键值对）
        if (device_id.empty())
        {
            // 生成一个随机的设备ID
            device_id = CommonService::GetRandomId();
            // 将新生成的设备ID写入注册表
            WriteRegister(kDeviceIdKey, device_id);
        }
        // 返回设备ID
        return device_id;
    }

    // 获取生命周期ID的函数
    static std::string GetLifeCycleId()
    {
        // 生成一个随机的生命周期ID
        static std::string life_cycle_id = CommonService::GetRandomId();
        // 返回生命周期ID
        return life_cycle_id;
    }

    // 获取系统版本的函数
    static std::string GetSystemVersion()
    {
        // 用于存储系统版本信息的结构体
        OSVERSIONINFOEXA os_version_info;
        // 初始化结构体
        ZeroMemory(&os_version_info, sizeof(OSVERSIONINFOEXA));
        // 设置结构体的大小
        os_version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
        // 获取系统版本信息
        ::GetVersionExA(reinterpret_cast<OSVERSIONINFOA*>(&os_version_info));
        // 将系统版本信息转换为字符串
        std::string system_version =
            std::to_string(os_version_info.dwMajorVersion) + "." +
            std::to_string(os_version_info.dwMinorVersion) + "." +
            std::to_string(os_version_info.dwBuildNumber);
        // 返回系统版本
        return system_version;
    }

    // 获取设备名称的函数
    static std::string GetDeviceName()
    {
        // 用于存储设备名称的缓冲区
        char buf[1024] = {0};
        // 缓冲区的大小
        DWORD buf_size = sizeof(buf);
        // 获取设备名称
        ::GetComputerNameA(buf, &buf_size);
        // 将设备名称转换为字符串
        std::string device_name = buf;
        // 返回设备名称
        return device_name;
    }

    // 获取进程创建时间的函数
    std::string CommonService::GetProcessTime()
    {
        // 获取当前进程的ID
        DWORD pid = ::GetCurrentProcessId();
        // 打开当前进程
        HANDLE h_process =
            ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        // 如果打开进程失败，返回空字符串
        if (h_process == NULL)
        {
            return "";
        }
        // 用于存储进程创建时间、退出时间、内核时间和用户时间的变量
        FILETIME create_time;
        FILETIME exit_time;
        FILETIME kernel_time;
        FILETIME user_time;
        // 获取进程时间
        BOOL ret = ::GetProcessTimes(h_process, &create_time, &exit_time,
                                     &kernel_time, &user_time);
        // 关闭进程句柄
        ::CloseHandle(h_process);
        // 如果获取进程时间失败，返回空字符串
        if (ret == 0)
        {
            return "";
        }

        // 将文件时间转换为本地文件时间
        FILETIME create_local_time;
        ::FileTimeToLocalFileTime(&create_time, &create_local_time);

        // 将文件时间转换为系统时间
        SYSTEMTIME create_sys_time;
        ::FileTimeToSystemTime(&create_local_time, &create_sys_time);
        // 用于存储格式化的时间字符串的缓冲区
        char buf[128] = {0};
        // 将系统时间格式化为字符串
        sprintf_s(buf, "%04d-%02d-%02d %02d:%02d:%02d.%03d", create_sys_time.wYear,
                  create_sys_time.wMonth, create_sys_time.wDay, create_sys_time.wHour,
                  create_sys_time.wMinute, create_sys_time.wSecond,
                  create_sys_time.wMilliseconds);
        // 返回进程创建时间
        return buf;
    }


    // 获取当前日期的函数
    std::string CommonService::GetNowDate()
    {
        // 获取当前系统时间
        auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        // 将系统时间转换为字符串
        return std::ctime(&t);
    }

    // 生成随机ID的函数
    std::string CommonService::GetRandomId()
    {
        // 随机ID的长度
        static constexpr size_t len = 32;
        // 随机ID的字符集
        static constexpr auto chars =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        // 随机数生成器
        static std::mt19937_64 rng{std::random_device{}()};
        // 均匀分布的随机数生成器，用于生成0到60的随机数
        static std::uniform_int_distribution<size_t> dist{0, 60};
        // 用于存储随机ID的字符串
        std::string result;
        // 预分配字符串的空间，提高性能
        result.reserve(len);
        // 生成随机ID
        std::generate_n(std::back_inserter(result), len,
                        [&] { return chars[dist(rng)]; });
        // 返回随机ID
        return result;
    }

    // 初始化CommonService类的一些成员变量的函数
    void CommonService::Init()
    {
        // 获取系统版本
        system_version = GetSystemVersion();
        // 获取设备名称
        device_name = GetDeviceName();
        // 获取设备ID
        device_id = GetDeviceId();
        // 获取跟踪器版本
        tracker_version = PROJECT_VER;
        // 获取生命周期ID
        lifecycle_id = GetLifeCycleId();
    }
}
