target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        dacDma.cpp
        adcDma.cpp
    PUBLIC
        FILE_SET HEADERS
        FILES 
            adcDma.hpp
            dacDma.hpp
            led.hpp
)