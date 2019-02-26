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

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t lives;
    uint8_t level;
    uint8_t stage;
    uint32_t score;
} player_t;

typedef enum {
    mario_none      = 0b00000000u,
    mario_right     = 0b00000001u,
    mario_left      = 0b00000010u,
    mario_jump      = 0b00000100u,
    mario_hammer    = 0b00001000u,
    mario_run       = 0b00010000u,
    mario_climb     = 0b00100000u,
    mario_climb_end = 0b01000000u,
} mario_flags_t;

player_t* player1(void);

player_t* player2(void);

void player1_header_update(uint32_t ticks);

void player2_header_update(uint32_t ticks);