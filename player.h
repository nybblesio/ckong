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
#include <stdbool.h>
#include <stdint.h>

typedef struct player {
    uint8_t lives;
    uint8_t level;
    uint16_t score;
} player_t;