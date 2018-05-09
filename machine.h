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

typedef struct machine {
    uint32_t high_score;
    uint8_t credits;
} machine_t;

void machine_init(void);

machine_t* machine(void);

void machine_load_config(void);

void machine_save_config(void);

void machine_header_update(void);
