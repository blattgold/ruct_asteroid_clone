cmake_minimum_required(VERSION 3.20)
project(ruct_asteroid_clone VERSION 1.0 LANGUAGES C)

set(SOURCES main.c player.c util.c world.c)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

LINK_LIBRARIES(raylib m)

add_executable(asteroid ${SOURCES})
