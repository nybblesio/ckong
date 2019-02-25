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

typedef struct {
    char name[8];
    uint32_t score;
} high_score_t;

typedef struct {
    uint8_t credits[2];
    uint32_t high_score;
    high_score_t score_history[5];
} machine_t;

typedef struct {
    char header[8];
    machine_t machine;
} machine_file_t;

void machine_init(void);

void machine_load(void);

void machine_save(void);

machine_t* machine(void);

void machine_header_update(void);
