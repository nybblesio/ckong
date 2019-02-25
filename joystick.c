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

#include <assert.h>
#include <SDL_system.h>
#include <SDL_gamecontroller.h>
#include "joystick.h"

static uint16_t s_button_state = 0;
static const Uint8* s_keyboard_state;

bool joystick_button(
        joystick_t* joy,
        joystick_button_t button) {
    bool button_pressed = false;

    if (joy->controller != NULL) {
        button_pressed = SDL_GameControllerGetButton(
            joy->controller,
            (SDL_GameControllerButton) button) != 0;
    }

    if (!button_pressed) {
        switch (button) {
            case button_a:
                button_pressed = s_keyboard_state[SDL_SCANCODE_LCTRL];
                break;
            case button_b:
                button_pressed = s_keyboard_state[SDL_SCANCODE_RCTRL];
                break;
            case button_x:
                button_pressed = s_keyboard_state[SDL_SCANCODE_X];
                break;
            case button_y:
                button_pressed = s_keyboard_state[SDL_SCANCODE_Y];
                break;
            case button_back:
                button_pressed = s_keyboard_state[SDL_SCANCODE_BACKSPACE];
                break;
            case button_guide:
                button_pressed = s_keyboard_state[SDL_SCANCODE_F1];
                break;
            case button_start:
                button_pressed = s_keyboard_state[SDL_SCANCODE_RETURN];
                break;
            case button_left_stick:
                break;
            case button_right_stick:
                break;
            case button_left_shoulder:
                button_pressed = s_keyboard_state[SDL_SCANCODE_LSHIFT];
                break;
            case button_right_shoulder:
                button_pressed = s_keyboard_state[SDL_SCANCODE_RSHIFT];
                break;
            case button_dpad_up:
                button_pressed = s_keyboard_state[SDL_SCANCODE_UP];
                break;
            case button_dpad_down:
                button_pressed = s_keyboard_state[SDL_SCANCODE_DOWN];
                break;
            case button_dpad_left:
                button_pressed = s_keyboard_state[SDL_SCANCODE_LEFT];
                break;
            case button_dpad_right:
                button_pressed = s_keyboard_state[SDL_SCANCODE_RIGHT];
                break;
            default:
                break;
        }
    }

    return button_pressed;
}

bool joystick_button_pressed(
        joystick_t* joy,
        joystick_button_t button) {
    const uint32_t bit_mask = (uint32_t)1 << (uint32_t)button;
    for (int8_t i = 0; i < 32; i++) {
        bool pressed = joystick_button(joy, button);
        if (pressed) {
            if ((s_button_state & bit_mask) == 0) {
                s_button_state |= bit_mask;
            }
        } else {
            if ((s_button_state & bit_mask) == bit_mask) {
                s_button_state &= ~bit_mask;
                return true;
            }
        }
    }
    return false;
}

joystick_t* joystick_open() {
    s_keyboard_state = SDL_GetKeyboardState(NULL);

    joystick_t* joy = malloc(sizeof(joystick_t));
    joy->index = -1;
    joy->name = NULL;
    joy->mapping = NULL;
    joy->controller = NULL;

    int32_t number_of_joysticks = SDL_NumJoysticks();
    for (int32_t i = 0; i < number_of_joysticks; ++i) {
        if (SDL_IsGameController(i) == SDL_TRUE) {
            joy->index = i;
            joy->controller = SDL_GameControllerOpen(i);
            joy->name = SDL_GameControllerNameForIndex(i);
            joy->mapping = SDL_GameControllerMappingForIndex(i);
            break;
        }
    }

    return joy;
}

void joystick_close(joystick_t* joy) {
    if (joy == NULL)
        return;

    if (joy->controller != NULL)
        SDL_GameControllerClose(joy->controller);

    if (joy->mapping != NULL)
        SDL_free((void*) joy->mapping);

    if (joy->name != NULL)
        SDL_free((void*) joy->name);
}
