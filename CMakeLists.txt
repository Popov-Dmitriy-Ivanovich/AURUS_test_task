cmake_minimum_required(VERSION 3.10)
project(AURUS_test)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
include_directories(.)
include_directories(${src})
add_executable(AURUS_RUN naive.cpp src/core/file_reader.cpp)

include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)


include(GoogleTest)

add_executable(AURUS_TEST tests/main.cpp src/core/file_reader.cpp)

target_link_libraries(AURUS_TEST GTest::gtest_main)

gtest_discover_tests(AURUS_TEST)