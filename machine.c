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
#include "machine.h"
#include "video.h"

static machine_t s_machine;

void machine_init(void) {
    s_machine.credits = 0;
    s_machine.high_score = 998779;
}

machine_t* machine(void) {
    return &s_machine;
}

void machine_load_config(void) {
    // XXX: read config file
}

void machine_save_config(void) {
    // XXX: write config file
}

void machine_header_update(void) {
    video_bg_str("HIGH SCORE", 0, 11, 0, true);
    char buffer[7];
    snprintf(&buffer[0], 7, "%06d", s_machine.high_score);
    video_bg_str(buffer, 1, 13, 1, true);
}
