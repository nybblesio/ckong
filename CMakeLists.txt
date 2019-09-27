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

# dummy target used for file copies
add_custom_target(dummy-target ALL DEPENDS custom-output)
add_custom_command(OUTPUT custom-output COMMAND ${CMAKE_COMMAND} -E echo DEPENDS always-rebuild)
add_custom_command(OUTPUT always-rebuild COMMAND ${CMAKE_COMMAND} -E echo)

# freetype
add_subdirectory(ext/freetype-2.9)
include_directories(ext/freetype-2.9/include)

# sdl2
set(SDL_SHARED OFF)
add_subdirectory(ext/SDL2-2.0.10)
include_directories(ext/SDL2-2.0.10/include)

# sdl2_ttf
add_subdirectory(ext/SDL2_ttf-2.0.14)
include_directories(ext/SDL2_ttf-2.0.14)

# SDL_FontCache
include_directories(ext/SDL_FontCache)

# inih
include_directories(ext/inih-42)

add_executable(
        ckong
        fwd.h
        main.c
        log.c log.h
        str.c str.h
        game.c game.h
        tile.c tile.h
        actor.c actor.h
        video.c video.h
        level.c level.h
        timer.c timer.h
        player.c player.h
        sprite.c sprite.h
        window.c window.h
        palette.c palette.h
        machine.c machine.h
        tile_map.c tile_map.h
        keyboard.c keyboard.h
        joystick.c joystick.h
        linked_list.c linked_list.h
        state_machine.c state_machine.h

        ext/inih-42/ini.c ext/inih-42/ini.h
        ext/SDL_FontCache/SDL_FontCache.c ext/SDL_FontCache/SDL_FontCache.h)

target_link_libraries(
        ckong
        SDL2_ttf
        SDL2-static)

add_custom_target(ckong-configured DEPENDS dummy-target ckong)
add_custom_command(
        TARGET ckong-configured
        COMMAND ${CMAKE_COMMAND} -E echo  "make_directory: ${PROJECT_BINARY_DIR}/logs"
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/logs

        COMMAND ${CMAKE_COMMAND} -E echo "copy_directory: ${PROJECT_SOURCE_DIR}/assets to ${PROJECT_BINARY_DIR}/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${PROJECT_BINARY_DIR}/assets
)
