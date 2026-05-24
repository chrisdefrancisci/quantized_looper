cmake_minimum_required(VERSION 3.22)
project(QuantizedLooper VERSION 0.1.0)

enable_testing()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message("Build type: " ${CMAKE_BUILD_TYPE})
enable_language(C CXX ASM)

find_package(Git QUIET)
if(Git_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/reusable_synth/reusable_synth/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

if(CMAKE_CROSSCOMPILING)
    message("Cross compiling for STM32")
    add_executable(${CMAKE_PROJECT_NAME})

    target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        ./quantized_looper/main.cpp
    )

    add_subdirectory(quantized_looper/cmake/stm32cubemx)

    target_link_libraries(${CMAKE_PROJECT_NAME}
        stm32cubemx
    )

    find_library(REUSABLE_SYNTH
        NAMES ReusableSynthInterface
        HINTS extern/reusable_synth/build/Debug
    )

    target_link_libraries(${CMAKE_PROJECT_NAME}
        ${REUSABLE_SYNTH}
    )

    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/extern/reusable_synth
    )

    add_subdirectory(${CMAKE_SOURCE_DIR}/quantized_looper)

else()
    message("Local build for QuantizedLooperTests")

    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(${CMAKE_PROJECT_NAME})

    add_subdirectory(tests)

    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/extern/reusable_synth
    )

    target_link_libraries(${CMAKE_PROJECT_NAME}
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(${CMAKE_PROJECT_NAME})
endif()

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)