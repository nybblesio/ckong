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
#include "video.h"
#include "actor.h"

static level_t s_levels[] = {
    {
        .tile_map = tile_map_introduction
    }
};

static bool help_blinker_callback(bg_blinker_t* blinker) {
    actor_t* pauline = actor(actor_pauline);
    if (blinker->visible) {
        blinker->duration = 1000;
        actor_animation(pauline, anim_pauline_shuffle_right);
    } else {
        blinker->duration = 5000;
        actor_animation(pauline, anim_pauline_stand_right);
    }
    return true;
}

level_t* level(uint8_t number) {
    return &s_levels[number];
}

void level_header_update(void) {
    unsigned char buffer[4] = {0xed, 0xee, 0xef, 0x00};
    video_bg_str((const char*) &buffer[0], 4, 15, 6, false);
    video_bg_blink(
        4,
        15,
        1,
        3,
        5000,
        help_blinker_callback);
}
