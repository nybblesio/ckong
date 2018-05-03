#pragma once

#include <stdint.h>
#include <stdlib.h>

static const uint8_t tile_width = 8;
static const uint8_t tile_height = 8;
static const uint16_t tile_max = 256;

typedef struct tile_bitmap {
    uint8_t data[tile_height * tile_width];
} tile_bitmap_t;

const tile_bitmap_t* tile_bitmap(uint16_t tile);