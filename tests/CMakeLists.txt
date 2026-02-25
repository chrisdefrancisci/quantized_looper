cmake_minimum_required(VERSION 3.22)
project(quantized_looper_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(mocks)

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/hello_test.cpp 
    # PUBLIC
        # FILE_SET HEADERS 
        # FILES 
)

