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

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "tile_map.h"

typedef struct level {
    uint8_t number;
    tile_maps_t tile_map;
} level_t;

level_t* level(uint8_t number);