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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "log.h"
#include "video.h"
#include "machine.h"

static machine_file_t s_file;

void machine_init(void) {
    strncpy(s_file.header, "CKONG11*", 8);
    s_file.machine.credits = 0;

    char name_char = 'A';
    uint32_t score = 10000;
    for (uint8_t i = 0; i < 5; i++) {
        s_file.machine.score_history[i].score = score;

        memset(s_file.machine.score_history[i].name, name_char++, 6);
        s_file.machine.score_history[i].name[7] = '\0';

        score -= 1000;
    }

    s_file.machine.high_score = s_file.machine.score_history[0].score;
}

void machine_load(void) {
    if (access("machine.dat", F_OK) == -1) {
        log_warn(category_app, "machine.dat file is missing; not loading.");
        return;
    }
    FILE* file = fopen("machine.dat", "rb");
    fread(&s_file, sizeof(machine_file_t), 1, file);
    fclose(file);
}

void machine_save(void) {
    FILE* file = fopen("machine.dat", "wb");
    fwrite(&s_file, sizeof(machine_file_t), 1, file);
    fclose(file);
}

machine_t* machine(void) {
    return &s_file.machine;
}

void machine_header_update(void) {
    video_bg_str(1, 13, 1, true, "%06d", s_file.machine.high_score);
}
