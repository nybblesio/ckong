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

#include <stdio.h>
#include "level.h"

static level_t s_levels[] = {
    {
        .tile_map = tile_map_introduction
    }
};

level_t* level(uint8_t number) {
    return &s_levels[number];
}

void level_header_update(void) {
}
