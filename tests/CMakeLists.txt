add_subdirectory(mocks)

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/hello_test.cpp 
    # PUBLIC
        # FILE_SET HEADERS 
        # FILES 
)

