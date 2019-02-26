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

#include <SDL_timer.h>
#include "timer.h"
#include "log.h"

static timer_t s_timers[128];

static uint32_t s_current_timer;

static timer_t* find_free_timer() {
    for (uint8_t i = 0; i < s_current_timer; i++) {
        if (!s_timers[i].active)
            return &s_timers[i];
    }

    if (s_current_timer < MAX_TIMERS)
        return &s_timers[s_current_timer++];

    return NULL;
}

void timer_init() {
    s_current_timer = 0;

    for (uint8_t i = 0; i < MAX_TIMERS; i++) {
        s_timers[i].id = i;
        s_timers[i].active = false;
    }
}

timer_t* timer_start(
        uint32_t ticks,
        uint32_t duration,
        timer_callback_t callback,
        void* user) {
    timer_t* timer = find_free_timer();
    if (timer == NULL)
        return NULL;

    timer->user = user;
    timer->active = true;
    timer->callback = callback;
    timer->duration = duration;
    timer->expiry_ticks = ticks + duration;

    return timer;
}

void timer_stop(timer_t* timer) {
    if (timer == NULL)
        return;
    timer->active = false;
}

void timer_update(uint32_t ticks) {
    for (uint8_t i = 0; i < s_current_timer; i++) {
        timer_t* timer = &s_timers[i];

        if (!timer->active)
            continue;

        if (ticks > timer->expiry_ticks) {
            if (timer->callback == NULL) {
                timer->active = false;
                continue;
            }

            bool kill = !timer->callback(timer, ticks);
            if (kill) {
                timer->active = false;
            } else {
                timer->expiry_ticks = ticks + timer->duration;
            }
        }
    }
}
