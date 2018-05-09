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

typedef struct level level_t;
typedef struct player player_t;
typedef struct machine machine_t;
typedef struct game_controller game_controller_t;

typedef struct state_context {
    level_t* level;
    player_t* player;
    machine_t* machine;
    game_controller_t* controller;
} state_context_t;

typedef bool (*state_machine_callback_t)(state_context_t*);

typedef enum states {
    state_boot,
    state_attract,
    state_long_introduction,
    state_how_high,
    state_game,
    state_round_won,
    state_donkey_kong_flees,
    state_tile_map_editor
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