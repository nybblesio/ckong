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

#include <SDL_keyboard.h>
#include "keyboard.h"

static bool s_key_pressed[SDL_NUM_SCANCODES];

bool key_state(uint32_t code) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    return state[code];
}

bool key_pressed(uint32_t code) {
    for (int8_t i = 0; i < 32; i++) {
        bool pressed = key_state(code);
        if (pressed) {
            if (!s_key_pressed[code]) {
                s_key_pressed[code] = true;
            }
        } else {
            if (s_key_pressed[code]) {
                s_key_pressed[code] = false;
                return true;
            }
        }
    }
    return false;
}