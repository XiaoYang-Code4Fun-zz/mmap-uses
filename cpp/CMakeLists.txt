cmake_minimum_required (VERSION 3.0)

project(mmap-cpp)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -O2")

include_directories(include)

file(GLOB SOURCES "src/*.cpp" "src/*.h") 

add_executable(perf_test ${SOURCES})
