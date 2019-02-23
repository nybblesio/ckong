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

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

struct _SDL_GameController;

typedef enum game_controller_button {
    invalid = -1,
    button_a,
    button_b,
    button_x,
    button_y,
    button_back,
    button_guide,
    button_start,
    button_left_stick,
    button_right_stick,
    button_left_shoulder,
    button_right_shoulder,
    button_dpad_up,
    button_dpad_down,
    button_dpad_left,
    button_dpad_right,
} game_controller_button_t;

typedef struct game_controller {
    int32_t index;
    const char* name;
    const char* mapping;
    struct _SDL_GameController* controller;
} game_controller_t;

bool game_controller_button(
    game_controller_t* controller,
    game_controller_button_t button);

bool game_controller_button_pressed(
    game_controller_t* controller,
    game_controller_button_t button);

game_controller_t* game_controller_open();

void game_controller_close(game_controller_t* controller);
