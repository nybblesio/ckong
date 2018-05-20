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

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "linked_list.h"

struct SDL_Window;
struct SDL_Surface;
struct SDL_Renderer;
struct SDL_Texture;

#define SCALE_X (4)
#define SCALE_Y (4)
#define SCREEN_WIDTH (256)
#define SCREEN_HEIGHT (256)

typedef struct window {
    bool valid;
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    uint32_t scale_x;
    uint32_t scale_y;
    struct SDL_Window* window;
    struct SDL_Surface* surface;
    struct SDL_Texture* texture;
    linked_list_node_t* messages;
    struct SDL_Renderer* renderer;
} window_t;

window_t window_create();
