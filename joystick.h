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
#include "fwd.h"

typedef enum {
    button_invalid = -1,
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
} joystick_button_t;

typedef struct {
    int32_t index;
    const char* name;
    const char* mapping;
    struct _SDL_GameController* controller;
} joystick_t;

bool joystick_button(
    joystick_t* joy,
    joystick_button_t button);

bool joystick_button_pressed(
    joystick_t* joy,
    joystick_button_t button);

joystick_t* joystick_open();

void joystick_close(joystick_t* joy);