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
#include <stddef.h>
#include <stdbool.h>

#define MAX_TIMERS (128)
#define SECONDS(s) ((1000 * s))

typedef struct timer timer_t;
typedef bool (*timer_callback_t)(timer_t*, uint32_t);

typedef struct timer {
    uint8_t id;
    void* user;
    bool active;
    uint32_t duration;
    uint32_t expiry_ticks;
    timer_callback_t callback;
} timer_t;

void timer_init();

timer_t* timer_start(
    uint32_t ticks,
    uint32_t duration,
    timer_callback_t callback,
    void* user);

void timer_stop(timer_t* timer);

void timer_update(uint32_t ticks);
