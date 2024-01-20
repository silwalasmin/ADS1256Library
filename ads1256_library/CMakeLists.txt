cmake_minimum_required(VERSION 3.22)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Initializes the Pico SDK
pico_sdk_init()

# Project name with version
project(ads1256
        VERSION 1.0
        DESCRIPTION "ADS1256 stand-alone library for RPi Pico"
        LANGUAGES C
)

# Versions of the used languages
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
enable_language(C CXX)

# Adds the CMakeLists.txt file from the ./src directory
# Include source file(s) to project
add_library(ads1256 STATIC
        src/ADS1256.cpp
)

# Links all required dependencies
target_link_libraries(ads1256
        pico_stdlib
        hardware_spi
)

# Link include directory / Path to header file
target_include_directories(ads1256 PUBLIC inc)