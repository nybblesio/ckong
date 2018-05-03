// --------------------------------------------------------------------------
//
// C Kong
// Copyright (C) 2018 Jeff Panici
// All rights reserved.
//
// This software source file is licensed according to the
// MIT License.  Refer to the LICENSE file distributed along
// with this source file to learn more.
//
// --------------------------------------------------------------------------

#include <SDL2/SDL.h>
#include "window.h"
#include "str.h"

window_t window_create() {
    window_t result;
    result.valid = false;
    result.messages = linked_list_new_node();

    result.scale_x = 3;
    result.scale_y = 3;
    result.width = screen_width;
    result.height = screen_height;
    result.window = SDL_CreateWindow(
        "C11 Kong",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        result.width * result.scale_x,
        result.height * result.scale_y,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (result.window == NULL) {
        result.messages->data = str_clone("Unable to create SDL window.");
        return result;
    }

    //result.surface = SDL_GetWindowSurface(result.window);
    result.surface = NULL;

    result.renderer = SDL_CreateRenderer(
        result.window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (result.renderer == NULL) {
        result.messages->data = str_clone("Unable to create SDL renderer.");
        return result;
    }

    int x, y;
    SDL_GetWindowPosition(result.window, &x, &y);
    result.x = (uint32_t) x;
    result.y = (uint32_t) y;

    result.valid = true;
    return result;
}
