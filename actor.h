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
#include <stdbool.h>

typedef enum {
    actor_bonus,
    actor_heart,
    actor_mario,
    actor_pauline,
    actor_oil_fire,
    actor_oil_barrel,
    actor_donkey_kong,
} actors_t;

typedef enum {
    anim_none,
    anim_oil_fire,
    anim_bonus_100,
    anim_bonus_200,
    anim_bonus_300,
    anim_bonus_500,
    anim_bonus_800,
    anim_mario_die,
    anim_oil_barrel,
    anim_mario_climb,
    anim_barrel_stacked,
    anim_mario_climb_end,
    anim_mario_walk_left,
    anim_mario_jump_left,
    anim_mario_jump_right,
    anim_mario_walk_right,
    anim_mario_stand_left,
    anim_barrel_roll_left,
    anim_barrel_roll_down,
    anim_donkey_kong_roar,
    anim_donkey_kong_stand,
    anim_mario_stand_right,
    anim_barrel_roll_right,
    anim_pauline_stand_left,
    anim_pauline_stand_right,
    anim_pauline_shuffle_left,
    anim_pauline_shuffle_right,
    anim_donkey_kong_title_pose,
    anim_mario_hammer_walk_left,
    anim_mario_hammer_walk_right,
    anim_donkey_kong_throw_barrel,
    anim_donkey_kong_climb_ladder,
} animations_t;

typedef enum {
    f_actor_none    = 0b00000000,
    f_actor_enabled = 0b00000001,
} actor_flags_t;

typedef struct {
    int16_t x_offset;
    int16_t y_offset;
    uint16_t tile;
    uint8_t palette;
    uint8_t flags;
} animation_frame_tile_t;

typedef struct {
    uint16_t delay;
    uint8_t tile_count;
    animation_frame_tile_t tiles[32];
} animation_frame_t;

typedef struct {
    uint8_t frame_count;
    animation_frame_t frames[32];
} animation_t;

typedef struct actor actor_t;
typedef bool (*actor_anim_callback_t)(actor_t*);

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
    actor_anim_callback_t animation_callback;
} actor_t;

actor_t* actor(actors_t actor);

void actor_reset();

void actor_update(uint32_t ticks);

void actor_animation(actor_t* actor, animations_t animation, uint32_t ticks);