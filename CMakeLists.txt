cmake_minimum_required(VERSION 3.11)
project(ckong C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

if ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_C_FLAGS "-Wall" CACHE STRING "compile flags" FORCE)
    message(STATUS "Using clang flags: ${CMAKE_C_FLAGS}")
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "GNU")
    set(CMAKE_C_FLAGS "-Wall" CACHE STRING "compile flags" FORCE)
    message(STATUS "Using gcc flags: ${CMAKE_C_FLAGS}")
else ()
    message(STATUS "Unknown compiler; no custom flags set: ${CMAKE_C_COMPILER_ID}")
endif()

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
        log.c log.h
        str.c str.h
        game.c game.h
        tile.c tile.h
        actor.c actor.h
        video.c video.h
        level.c level.h
        player.c player.h
        sprite.c sprite.h
        window.c window.h
        palette.c palette.h
        machine.c machine.h
        tile_map.c tile_map.h
        linked_list.c linked_list.h
        state_machine.c state_machine.h
        game_controller.c game_controller.h)

target_link_libraries(
        ckong
        ${SDL2_LIBRARY}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES})

