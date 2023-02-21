cmake_minimum_required(VERSION 3.1)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_CXX_STANDARD 20)

project(HansLib)

add_subdirectory(Utilities)
add_subdirectory(UtilitiesTest)

