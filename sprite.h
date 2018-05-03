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

static const uint8_t sprite_width = 16;
static const uint8_t sprite_height = 16;
static const uint16_t sprite_max = 128;

typedef struct sprite_bitmap {
    uint8_t data[sprite_height * sprite_width];
} sprite_bitmap_t;

const sprite_bitmap_t* sprite_bitmap(uint16_t tile);