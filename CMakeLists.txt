cmake_minimum_required(VERSION 3.11)
project(ckong C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(
        ckong
        main.c
        str.c str.h
        palette.c palette.h
        tile.c tile.h
        sprite.c sprite.h
        actor.c actor.h
        tile_map.c tile_map.h)