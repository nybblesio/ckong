#pragma once

#include <stdint.h>

const uint8_t tile_width = 8;
const uint8_t tile_height = 8;
const uint16_t tile_max = 256;

typedef struct tile_bitmap {
    uint8_t data[tile_height * tile_width];
} tile_bitmap_t;