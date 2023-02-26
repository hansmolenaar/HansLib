cmake_minimum_required(VERSION 3.10)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(BUILD_SHARED_LIBS OFF)
set (CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_INCLUDE_DIR ${CMAKE_SOURCE_DIR})

message("CMAKE_BINARY_DIR" ${CMAKE_BINARY_DIR})
message("CMAKE_SOURCE_DIR" ${CMAKE_SOURCE_DIR})
message("CMAKE_INCLUDE_DIR" ${CMAKE_INCLUDE_DIR})
message("CMAKE_RUNTIME_OUTPUT_DIRECTORY " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

project(HansLib)

# Setup testing
set(GTestLibs GTest::GTest GTest::Main)
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_subdirectory(Utilities)
add_subdirectory(UtilitiesTest)

add_subdirectory(Point)
add_subdirectory(PointTest)

add_subdirectory(Interval)
add_subdirectory(IntervalTest)

add_subdirectory(BoundingBox)
add_subdirectory(BoundingBoxTest)


