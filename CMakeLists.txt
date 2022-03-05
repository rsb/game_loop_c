cmake_minimum_required(VERSION 3.21)
project(game_loop C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(game_loop
        src/main.c)
