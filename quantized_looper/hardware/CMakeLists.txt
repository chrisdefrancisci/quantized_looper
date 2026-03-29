target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        dac_singleton.cpp
    PUBLIC
        FILE_SET HEADERS
        FILES 
            dac_singleton.hpp
            led.hpp
)