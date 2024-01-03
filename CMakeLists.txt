cmake_minimum_required(VERSION 3.27)
project(CppTracker)

set(CMAKE_CXX_STANDARD 23)


set(PROJECT_NAME "CppTracker")

project(${PROJECT_NAME} VERSION 1.1.1.1)

option(BUILD_TRACKER_SHARED_LIBS "build shared library" ON)
option(BUILD_TRACKER_STATIC_LIBS "build static library" ON)

#option(BUILD_TRACKER_EXAMPLES "build examples" OFF)
#option(BUILD_TRACKER_TEST "build unittest" OFF)
option(BUILD_TRACKER_EXAMPLES "build examples" ON)
option(BUILD_TRACKER_TEST "build unittest" ON)

option(BUILD_TRACKER_FOR_MT "build for /MT" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

if (BUILD_TRACKER_FOR_MT)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif ()

set(LIBS ${LIBS} winmm iphlpapi ws2_32 dbghelp)

# generate pdb file
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE
        "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

set(INCDIRS . lib ${CMAKE_BINARY_DIR})
include_directories(${INCDIRS})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /std:c11")

message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DDEBUG)
else ()
    add_definitions(-DNDEBUG)
endif ()

add_subdirectory(src)

if (BUILD_TRACKER_EXAMPLES)
    add_subdirectory(examples)
endif ()

if (BUILD_TRACKER_TEST)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)

    add_subdirectory(tests)

endif ()
