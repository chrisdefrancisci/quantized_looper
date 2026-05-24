add_subdirectory(mocks)
add_subdirectory(utils)

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/hello_test.cpp 
)

