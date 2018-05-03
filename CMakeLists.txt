cmake_minimum_required(VERSION 3.11)
project(ckong C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# rpath setup
SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH "/usr/local/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_executable(
        ckong
        main.c
        str.c str.h
        game.c game.h
        palette.c palette.h
        tile.c tile.h
        sprite.c sprite.h
        actor.c actor.h
        tile_map.c tile_map.h
        window.c window.h linked_list.c linked_list.h)

target_link_libraries(
        ckong
        ${SDL2_LIBRARY}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES})

