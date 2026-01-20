target_sources(quantized_looper
    # PRIVATE
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS 
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES 
            logger_singleton.hpp
)