cmake_minimum_required(VERSION 3.8)
project(hid_spi_redirect)

set(CMAKE_C_STANDARD 99)

find_package(Threads)

set(SOURCE_FILES main.c spi.h spi.c JoystickRead.h JoystickRead.c SpiBus.h SpiBus.c Xpad.h Xpad.c)
add_executable(hid_spi_redirect ${SOURCE_FILES})
target_link_libraries(hid_spi_redirect ${CMAKE_THREAD_LIBS_INIT})
