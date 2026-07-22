cmake_minimum_required(VERSION 3.16)
project(robot_firmware C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(robot_firmware
    src/main.c
    src/commands.c
    src/config.c
    src/hal_desktop.c
    src/telemetry.c
)

target_include_directories(robot_firmware PRIVATE include)

if(MSVC)
    target_compile_options(robot_firmware PRIVATE /W3)
else()
    target_compile_options(robot_firmware PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()
