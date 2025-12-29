cmake_minimum_required(VERSION 3.22)
project(quantized_looper_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  quantized_looper_tests
#   hello_test.cpp
)

add_subdirectory(mocks)
add_subdirectory(tests)

target_link_libraries(
  quantized_looper_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(quantized_looper_tests)
