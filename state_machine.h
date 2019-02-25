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
#include <stdlib.h>
#include "level.h"
#include "player.h"
#include "machine.h"
#include "joystick.h"

typedef struct state_context {
    level_t* level;
    player_t* player;
    machine_t* machine;
    joystick_t* joystick;
} state_context_t;

typedef bool (*state_machine_callback_t)(state_context_t*);

//
// bonus box under level number
//
// show "GAME OVER" text on tile map
//
// screens:
//
//  insert coin + high score table
//
//  long introduction
//
//  high how (probably all runtime generated)
//
//  game screen 1 (climb to pauline)
//  game screen 2
//  game screen 3
//  game screen 4
//
//  title screen
//
//  name registration
//
//
typedef enum states {
    state_boot,
    state_title,
    state_credit,
    state_how_high,
    state_high_score,
    state_insert_coin,
    state_game_screen_1,
    state_game_screen_2,
    state_game_screen_3,
    state_game_screen_4,
    state_editor,
    state_editor_menu,
    state_editor_pick_tile,
    state_editor_pick_palette,
    state_long_introduction,
} states_t;

typedef struct state {
    states_t state;
    state_machine_callback_t enter;
    state_machine_callback_t update;
    state_machine_callback_t leave;
} state_t;

void state_pop(state_context_t* context);

void state_update(state_context_t* context);

void state_push(state_context_t* context, states_t state);