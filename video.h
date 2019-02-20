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
#include "tile_map.h"

#define BLINKERS_MAX (16)
#define FRAME_RATE (60)
#define MS_PER_FRAME (1000 / FRAME_RATE)

struct SDL_Surface;

typedef enum spr_flags {
    f_spr_none     = 0b00000000,
    f_spr_enabled  = 0b00000001,
    f_spr_collided = 0b00000010,
    f_spr_hflip    = 0b00000100,
    f_spr_vflip    = 0b00001000,
    f_spr_changed  = 0b00010000,
} spr_flags_t;

typedef enum bg_flags {
    f_bg_none      = 0b00000000,
    f_bg_enabled   = 0b00000001,
    f_bg_hflip     = 0b00000010,
    f_bg_vflip     = 0b00000100,
    f_bg_changed   = 0b00001000,
    f_bg_select    = 0b00010000,
} bg_flags_t;

typedef struct rect {
    int16_t left;
    int16_t top;
    int16_t width;
    int16_t height;
} rect_t;

typedef struct bg_blinker bg_blinker_t;
typedef bool (*bg_blinker_callback)(bg_blinker_t*);

typedef struct bg_blinker {
    rect_t bounds;
    bool visible;
    uint32_t timeout;
    uint32_t duration;
    uint32_t data1;
    uint32_t data2;
    bg_blinker_callback callback;
} bg_blinker_t;

typedef struct bg_control_block {
    uint16_t tile;
    uint8_t flags;
    uint8_t palette;
    uint32_t data1;
    uint32_t data2;
} bg_control_block_t;

typedef struct spr_control_block {
    uint16_t y;
    uint16_t x;
    uint16_t tile;
    uint8_t flags;
    uint8_t palette;
    uint32_t data1;
    uint32_t data2;
} spr_control_block_t;

void video_bg_str(
    const char* str,
    uint8_t y,
    uint8_t x,
    uint8_t palette,
    bool enabled);

bg_blinker_t* video_bg_blink(
    uint8_t y,
    uint8_t x,
    uint8_t h,
    uint8_t w,
    uint32_t duration,
    bg_blinker_callback callback);

void video_init(void);

void video_update(void);

void video_shutdown(void);

void video_reset_bg(void);

void video_reset_sprites(void);

void video_clip_rect_clear(void);

void video_clip_rect(rect_t rect);

struct SDL_Surface* video_surface(void);

void video_set_bg(const tile_map_t* map);

spr_control_block_t* video_sprite(uint8_t number);

void video_fill_bg(uint16_t tile, uint8_t palette);

bg_control_block_t* video_tile(uint8_t y, uint8_t x);
