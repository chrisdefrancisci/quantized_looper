cmake_minimum_required(VERSION 3.22)
project(QuantizedLooper VERSION 0.1.0)

enable_testing()

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message("Build type: " ${CMAKE_BUILD_TYPE})
# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)

# Git submodules example 
# From https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html
find_package(Git QUIET)
if(Git_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
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
    # Create an executable object type
    add_executable(${CMAKE_PROJECT_NAME})
    
    # Add sources to executable
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user sources here
        ./quantized_looper/main.cpp
    )

    # Add STM32CubeMX generated sources
    add_subdirectory(quantized_looper/cmake/stm32cubemx)

    target_link_libraries(${CMAKE_PROJECT_NAME}
        stm32cubemx
    )
else()
    message("Local build for QuantizedLooperTests")
    # Create an executable object type
    add_executable(${CMAKE_PROJECT_NAME})
    add_subdirectory(tests)

    include(FetchContent)
    FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    FetchContent_MakeAvailable(googletest)

    target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    GTest::gtest_main
    )
    enable_testing()
    include(GoogleTest)
    gtest_discover_tests(${CMAKE_PROJECT_NAME})
endif()


# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

find_library(REUSABLE_SYNTH ReusableSynthInterface HINTS extern/reusable_synth/build/Debug)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    # Add user defined libraries
    ${REUSABLE_SYNTH}
)

# add_subdirectory(${CMAKE_SOURCE_DIR}/quantized_looper)

target_sources(${CMAKE_PROJECT_NAME}
    PUBLIC
        FILE_SET HEADERS 
        BASE_DIRS 
            ${CMAKE_CURRENT_SOURCE_DIR}
)
# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${CMAKE_SOURCE_DIR}/quantized_looper
    ${CMAKE_SOURCE_DIR}/extern/reusable_synth
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)