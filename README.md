# CppTracker: C++ Performance Monitoring and Data Analysis SDK

## 项目概述
CppTracker 是一个用于客户端应用性能监控和数据分析的 C++ SDK。它旨在提供一种有效的方法来收集和分析应用性能数据，帮助开发者优化其应用程序。虽然最初为 Windows 平台设计，但由于其基于 C++ 的构建，理论上可以通过适当的代码修改和 CMake 配置调整，实现对 Linux、macOS、Android 和 iOS 等平台的支持。

## 主要特性
1. **并发任务管理与调度**：通过集成 Boost.Asio 的 strand 模型优化线程池，CppTracker 能够有效管理和调度并发任务，从而防止数据竞争和死锁。这主要应用于 context 管理和报告跟踪模块。

2. **定时任务执行**：CppTracker 集成了 Boost.Asio 的 deadline_timer，提供了强大的定时器功能。这允许在异步编程环境中定时执行任务，主要应用于报告跟踪模块。

3. **数据存储与网络通信**：结合 SQLite 数据库进行数据存储和查询，同时通过 Boost.Asio 实现的 HTTP 协议网络通信确保数据的有效上传和下载。这些特性主要用于报告跟踪模块。

4. **安全与数据处理**：使用 Crypto++ 库进行数据加密，确保数据安全性。此外，CppTracker 使用 spdlog 库进行日志记录和 nlohmann::json 库处理 JSON 数据，提高数据处理的效率和安全性，这些主要集中于加密模块和报告跟踪模块。

## 安装与配置
首先，您需要通过以下命令使用 vcpkg 安装所需的依赖库：

```shell
vcpkg install gtest cryptopp sqlite3 sqlite-orm nlohmann_json spdlog boost
```

接着，配置 vcpkg 工具链，以便于 CMake 在构建过程中正确地定位和使用这些库。在 CMake 配置中指定 `CMAKE_TOOLCHAIN_FILE` 的路径，确保它指向您本地 vcpkg 的安装目录。例如：

```shell
-DCMAKE_TOOLCHAIN_FILE=D:\vcpkg\scripts\buildsystems\vcpkg.cmake
```

## 平台支持
- **Windows**：原生支持。
- **其他平台**（Linux、macOS、Android、iOS等）：需要去除 Windows 平台特有代码并调整 CMake 配置。

## 相关知识点
- C/C++ 语言及其新特性
- 线程池实现和管理
- 定时器机制
- 数据库操作（特别是 SQLite）
- 网络通信（HTTP/HTTPS 请求）
- 安全性考虑（加密与解密）
- 日志系统的设计和实现
- JSON 数据处理
- 文件系统和时间相关操作

