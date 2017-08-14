cmake_minimum_required (VERSION 3.0)

project(mmap-cpp)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -O2")

include_directories(include)

add_executable(file_appender_perf_test
src/file_appender_perf_test.cpp
src/vanilla_file_appender.h
src/vanilla_file_appender.cpp
src/mmap_file_appender.h
src/mmap_file_appender.cpp)

add_executable(file_reader_perf_test
src/file_reader_perf_test.cpp
src/vanilla_file_reader.h
src/vanilla_file_reader.cpp
src/mmap_file_reader.h
src/mmap_file_reader.cpp)
