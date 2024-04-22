cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(stepper_project C CXX ASM)

set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()
add_executable(exe main.c)

pico_enable_stdio_usb(exe 1)
pico_enable_stdio_uart(exe 0)

pico_add_extra_outputs(exe)
target_link_libraries(exe
    pico_stdlib
    pico_cyw43_arch_none
    hardware_pwm
    pico_bootrom
)
