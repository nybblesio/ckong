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

static const uint8_t palette_max = 64;

typedef struct palette_entry {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} palette_entry_t;

typedef struct palette {
    palette_entry_t entries[4];
} palette_t;

const palette_t* palette(uint8_t index);