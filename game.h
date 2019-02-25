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
#include "joystick.h"
#include "linked_list.h"

typedef struct  {
    bool valid;
    window_t window;
    ll_node_t* messages;
    joystick_t* joystick;
} game_context_t;

game_context_t* game_context_new();

bool game_run(game_context_t* context);

bool game_init(game_context_t* context);

void game_shutdown(game_context_t* context);
