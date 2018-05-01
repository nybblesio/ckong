#pragma once

#include <stdint.h>

const uint8_t sprite_width = 16;
const uint8_t sprite_height = 16;
const uint16_t sprite_max = 128;

typedef struct sprite_bitmap {
    uint8_t data[sprite_height * sprite_width];
} sprite_bitmap_t;