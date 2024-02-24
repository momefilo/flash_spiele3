cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
set(PICO_BOARD yd_rp2040)
project(flash_spiele3_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(flash_spiele3
flash_spiele3.c
)
add_subdirectory(./ili9341)
pico_add_extra_outputs(flash_spiele3)
target_link_libraries(flash_spiele3
							pico_stdlib
							ili9341
							hardware_flash
							hardware_sync)
