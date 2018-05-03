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

static const uint8_t tile_map_width = 32;
static const uint8_t tile_map_height = 32;

typedef enum tile_maps {
    long_introduction
} tile_maps_t;

typedef struct tile_map_entry {
    uint16_t tile;
    uint8_t palette;
    uint8_t flags;
} tile_map_entry_t;

typedef struct tile_map {
    tile_map_entry_t data[tile_map_height* tile_map_width];
} tile_map_t;

const tile_map_t* tile_map(tile_maps_t map);