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

#include <SDL.h>
#include "window.h"
#include "str.h"
#include "log.h"

window_t window_create(int32_t y, int32_t x) {
    window_t result;
    result.valid = false;
    result.messages = ll_new_node();

    log_message(category_video, "create SDL window.");
    result.scale_x = SCALE_X;
    result.scale_y = SCALE_Y;
    result.width = SCREEN_WIDTH;
    result.height = SCREEN_HEIGHT;
    result.window = SDL_CreateWindow(
        "C11 Kong",
        x == -1 ? SDL_WINDOWPOS_CENTERED : x,
        y == -1 ? SDL_WINDOWPOS_CENTERED : y,
        result.width * result.scale_x,
        result.height * result.scale_y,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (result.window == NULL) {
        result.messages->data = str_clone("unable to create SDL window.");
        return result;
    }

    //result.surface = SDL_GetWindowSurface(result.window);
    result.surface = NULL;

    log_message(category_video, "create SDL renderer: accelerated, vsync.");
    result.renderer = SDL_CreateRenderer(
        result.window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (result.renderer == NULL) {
        result.messages->data = str_clone("unable to create SDL renderer.");
        return result;
    }

    result.texture = SDL_CreateTexture(
        result.renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT);
    log_message(category_video,
                "SDL streaming texture: w=%d, h=%d",
                SCREEN_WIDTH,
                SCREEN_HEIGHT);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(
        result.renderer,
        SCREEN_WIDTH,
        SCREEN_HEIGHT);
    log_message(category_video,
                "scale quality hint: %s.",
                SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY));
    log_message(category_video,
                "render logical size: w=%d, h=%d",
                SCREEN_WIDTH,
                SCREEN_HEIGHT);

    int wx, wy;
    SDL_GetWindowPosition(result.window, &wx, &wy);
    result.x = (uint32_t) wx;
    result.y = (uint32_t) wy;
    log_message(category_video, "window position: x=%d, y=%d", result.x, result.y);
    log_message(category_video,
                "window size: w=%d, h=%d",
                result.width * result.scale_x,
                result.height * result.scale_y);

    result.valid = true;
    return result;
}
