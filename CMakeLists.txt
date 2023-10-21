cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(atari_pad C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD pico)

pico_sdk_init()

add_executable(atari_pad main.c usb_descriptors.c)
target_include_directories(atari_pad PRIVATE .)

target_link_libraries(atari_pad pico_stdlib tinyusb_device tinyusb_board)

pico_add_extra_outputs(atari_pad)