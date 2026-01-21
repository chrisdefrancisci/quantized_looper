target_sources(quantized_looper
    PRIVATE
        led_tasks.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS 
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES 
            led_tasks.hpp
)