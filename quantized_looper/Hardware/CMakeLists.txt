cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(QlHardware)

target_sources(QlHardware
    # PRIVATE
    # source files go here
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS 
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES 
            led.hpp
)