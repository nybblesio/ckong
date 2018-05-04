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

typedef enum actors {
    actor_mario,
} actors_t;

typedef enum animations {
    anim_none,
    anim_mario_stand_left,
    anim_mario_stand_right,
    anim_mario_walk_left,
    anim_mario_walk_right,
    anim_mario_jump_left,
    anim_mario_jump_right,
} animations_t;

typedef enum actor_flags {
    f_actor_none    = 0b00000000,
    f_actor_enabled = 0b00000001,
} actor_flags_t;

typedef struct animation_frame_tile {
    int16_t x_offset;
    int16_t y_offset;
    uint16_t tile;
    uint8_t palette;
    uint8_t flags;
} animation_frame_tile_t;

typedef struct animation_frame {
    uint16_t delay;
    uint8_t tile_count;
    animation_frame_tile_t tiles[32];
} animation_frame_t;

typedef struct animation {
    uint8_t frame_count;
    animation_frame_t frames[32];
} animation_t;

typedef struct actor {
    int16_t x;
    int16_t y;
    uint8_t frame;
    uint16_t data1;
    uint16_t data2;
    uint32_t next_tick;
    actor_flags_t flags;
    animation_t* animation;
    animations_t animation_type;
} actor_t;

void actor_update(void);

actor_t* actor(actors_t actor);

void actor_animation(actor_t* actor, animations_t animation);