

# CppTracker: C++ Performance Monitoring and Data Analysis SDK

## 项目概述
CppTracker 是一个专门为客户端应用设计的 C++ 性能监控和数据分析 SDK。它提供了高效的工具集来收集和分析应用性能数据，帮助开发者识别和优化应用程序的性能瓶颈。最初为 Windows 平台开发，CppTracker 基于 C++ 的可移植性，可以通过适当的代码修改和 CMake 配置调整，以支持 Linux、macOS、Android 和 iOS 等多种平台。

## 主要特性
- **并发任务管理与调度**：利用 Boost.Asio 的 strand 模型优化线程池管理，提升并发任务的执行效率，降低数据竞争和死锁的风险。
- **定时任务执行**：集成 Boost.Asio 的 deadline_timer，支持在异步环境下的定时任务执行，优化报告跟踪模块的性能。
- **数据存储与网络通信**：结合 SQLite 实现高效的数据存储和查询；通过 Boost.Asio 支持的 HTTP 协议完成数据的上传和下载。
- **安全与数据处理**：使用 Crypto++ 库加密数据以保障安全性；采用 spdlog 和 nlohmann::json 库进行日志记录和 JSON 数据处理，提高效率和安全性。

## 安装与配置
**vcpkg安装使用教程**：https://github.com/microsoft/vcpkg

**依赖安装**：
使用 vcpkg 安装所需的依赖库：
```shell
vcpkg install gtest cryptopp sqlite3 sqlite-orm nlohmann_json spdlog boost
```

**CMake 配置**：
设置 `CMAKE_TOOLCHAIN_FILE` 以确保 CMake 正确使用 vcpkg 安装的库：
```shell
-DCMAKE_TOOLCHAIN_FILE=<vcpkg_install_path>/scripts/buildsystems/vcpkg.cmake
```

**测试report服务**：
1. 运行服务端：
   ```shell
   python3 server.py
   ```
2. 执行 HttpTest.TestHttpReport

## 平台支持
- **Windows**：原生支持。
- **其他平台**（Linux、macOS、Android、iOS 等）：需移除 Windows 特有代码并调整 CMake 配置。

## 相关知识点
- C/C++ 语言及其新特性
- 线程池、定时器机制
- SQLite 数据库操作
- HTTP/HTTPS 网络通信
- 数据加密与解密
- 日志系统设计
- JSON 数据处理
- 文件系统和时间操作

