# 指定CMake的最低版本要求
cmake_minimum_required(VERSION 3.27)

# 定义项目名称
project(CppTracker)

# 设置C++标准为C++23
set(CMAKE_CXX_STANDARD 20)

# 设置项目名称变量
set(PROJECT_NAME "CppTracker")

# 重新定义项目，包括版本号
project(${PROJECT_NAME} VERSION 1.1.1.1)


# 提供选项以构建示例，当前默认打开
option(BUILD_TRACKER_EXAMPLES "build examples" OFF)

# 提供选项以构建单元测试，当前默认打开
option(BUILD_TRACKER_TEST "build unittest" ON)

# 设置编译器标志来启用多处理器编译
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

# 定义项目需要链接的库
set(LIBS ${LIBS} winmm iphlpapi ws2_32 dbghelp)

# 设置在Release模式下生成PDB文件的编译器标志
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE
        "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

# 设置包含目录
set(INCDIRS . lib ${CMAKE_BINARY_DIR})
include_directories(${INCDIRS})

# 设置C语言标准为C11
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /std:c11")

# 打印构建类型和系统名称
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")

# 根据构建类型添加宏定义
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DDEBUG)
else ()
    add_definitions(-DNDEBUG)
endif ()

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(SqliteOrm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(Boost)
find_package(cryptopp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

# 添加子目录src到项目
add_subdirectory(src)

# 如果开启了构建示例，添加子目录examples到项目
if (BUILD_TRACKER_EXAMPLES)
    add_subdirectory(examples)
endif ()

# 如果开启了构建测试，启用测试并添加子目录tests到项目
if (BUILD_TRACKER_TEST)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    add_subdirectory(tests)
endif ()
