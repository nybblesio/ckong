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

#define TILE_WIDTH (8)
#define TILE_HEIGHT (8)
#define TILE_SIZE (TILE_HEIGHT * TILE_WIDTH)
#define TILE_MAX (256)

typedef struct tile_bitmap {
    uint8_t data[TILE_SIZE];
} tile_bitmap_t;

const tile_bitmap_t* tile_bitmap(uint16_t tile);