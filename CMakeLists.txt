# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/jdcaputo/.local/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(cirque_glidepoint_rpi_pico_driver C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(cirque_glidepoint_rpi_pico_driver 
        cirque_glidepoint_rpi_pico_driver.cpp 
        cirque_driver.cpp
)

pico_set_program_name(cirque_glidepoint_rpi_pico_driver "cirque_glidepoint_rpi_pico_driver")
pico_set_program_version(cirque_glidepoint_rpi_pico_driver "0.1")

pico_enable_stdio_uart(cirque_glidepoint_rpi_pico_driver 0)
pico_enable_stdio_usb(cirque_glidepoint_rpi_pico_driver 1)

# Add the standard library to the build
target_link_libraries(cirque_glidepoint_rpi_pico_driver pico_stdlib)

# Add any user requested libraries
target_link_libraries(cirque_glidepoint_rpi_pico_driver
        hardware_spi
        hardware_pio
)

pico_add_extra_outputs(cirque_glidepoint_rpi_pico_driver)

