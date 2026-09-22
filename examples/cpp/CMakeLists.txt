cmake_minimum_required(VERSION 3.16)
project(stats_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(stats src/stats.cpp)
target_include_directories(stats PUBLIC include)

enable_testing()

add_executable(stats_test tests/stats_test.cpp)
target_link_libraries(stats_test PRIVATE stats)

add_test(NAME stats_test COMMAND stats_test)
