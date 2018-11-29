cmake_minimum_required(VERSION 3.11)
project(ckong C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# configure ccache if available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    message(STATUS "${PROJECT_NAME} using ccache.")
endif()

if ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_C_FLAGS "-Wall" CACHE STRING "compile flags" FORCE)
    message(STATUS "Using clang flags: ${CMAKE_C_FLAGS}")
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "GNU")
    set(CMAKE_C_FLAGS "-Wall" CACHE STRING "compile flags" FORCE)
    message(STATUS "Using gcc flags: ${CMAKE_C_FLAGS}")
else ()
    message(STATUS "Unknown compiler; no custom flags set: ${CMAKE_C_COMPILER_ID}")
endif()

# freetype
add_subdirectory(ext/freetype-2.9)
include_directories(ext/freetype-2.9/include)

# sdl2
set(SDL_SHARED OFF)
add_subdirectory(ext/SDL2-2.0.9)
include_directories(ext/SDL2-2.0.9/include)

# sdl2_ttf
add_subdirectory(ext/SDL2_ttf-2.0.14)
include_directories(ext/SDL2_ttf-2.0.14)

# SDL_FontCache
add_library(
        SDL_FontCache
        ext/SDL_FontCache/SDL_FontCache.c
        ext/SDL_FontCache/SDL_FontCache.h)
include_directories(ext/SDL_FontCache)

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
        SDL_FontCache
        SDL2_ttf
        SDL2-static)
