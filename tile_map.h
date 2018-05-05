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

#define TILE_MAP_WIDTH (32)
#define TILE_MAP_HEIGHT (32)
#define TILE_MAP_SIZE (TILE_MAP_HEIGHT * TILE_MAP_WIDTH)

typedef enum tile_maps {
    long_introduction
} tile_maps_t;

typedef struct tile_map_entry {
    uint16_t tile;
    uint8_t palette;
    uint8_t flags;
} tile_map_entry_t;

typedef struct tile_map {
    tile_map_entry_t data[TILE_MAP_SIZE];
} tile_map_t;

const tile_map_t* tile_map(tile_maps_t map);