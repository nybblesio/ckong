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
#include "video.h"
#include "player.h"

static player_t s_player1 = {
    .lives = 3,
    .level = 1,
    .stage = 1,
    .score = 692,
};

static player_t s_player2 = {
    .lives = 3,
    .level = 1,
    .stage = 1,
    .score = 0,
};

player_t* player1(void) {
    return &s_player1;
}

player_t* player2(void) {
    return &s_player2;
}

void player1_header_update(void) {
    video_bg_str(0, 5, 0, false, "1UP");
    video_bg_blink(0, 5, 1, 3, 250, NULL);

    video_bg_str(1, 3, 1, true, "%06d", s_player1.score);

    for (uint8_t i = 0; i < s_player1.lives; i++) {
        bg_control_block_t* block = video_tile(3, (uint8_t) (3 + i));
        block->tile = 0xff;
        block->palette = 2;
        block->flags |= f_bg_changed;
    }

    video_bg_str(3, 23, 2, true, "L=%02d", s_player1.level);
}

void player2_header_update(void) {
}
