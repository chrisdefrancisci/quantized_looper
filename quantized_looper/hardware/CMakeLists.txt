target_sources(quantized_looper
    # PRIVATE
    # source files go here
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS 
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES 
            led.hpp
)