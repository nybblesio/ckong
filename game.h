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
#include "window.h"

typedef enum mario_flags {
    mario_none      = 0b00000000u,
    mario_right     = 0b00000001u,
    mario_left      = 0b00000010u,
    mario_jump      = 0b00000100u,
    mario_hammer    = 0b00001000u,
    mario_run       = 0b00010000u,
    mario_climb     = 0b00100000u,
    mario_climb_end = 0b01000000u,
} mario_flags_t;

typedef struct linked_list_node linked_list_node_t;
typedef struct game_controller game_controller_t;

typedef struct game_context {
    bool valid;
    window_t window;
    linked_list_node_t* messages;
    game_controller_t* controller;
} game_context_t;

game_context_t* game_context_new();

bool game_run(game_context_t* context);

bool game_init(game_context_t* context);

void game_shutdown(game_context_t* context);
