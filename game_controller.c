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
#include "game_controller.h"

static const Uint8* s_keyboard_state;

bool game_controller_button(
        game_controller_t* controller,
        game_controller_button_t button) {
    bool button_pressed = false;

    if (controller->controller != NULL) {
        button_pressed = SDL_GameControllerGetButton(
            controller->controller,
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
                break;
            case button_guide:
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

game_controller_t* game_controller_open() {
    s_keyboard_state = SDL_GetKeyboardState(NULL);

    game_controller_t* controller = malloc(sizeof(game_controller_t));
    controller->index = -1;
    controller->name = NULL;
    controller->mapping = NULL;
    controller->controller = NULL;

    int32_t number_of_joysticks = SDL_NumJoysticks();
    for (int32_t i = 0; i < number_of_joysticks; ++i) {
        if (SDL_IsGameController(i) == SDL_TRUE) {
            controller->index = i;
            controller->controller = SDL_GameControllerOpen(i);
            controller->name = SDL_GameControllerNameForIndex(i);
            controller->mapping = SDL_GameControllerMappingForIndex(i);
            break;
        }
    }

    return controller;
}

void game_controller_close(game_controller_t* controller) {
    if (controller == NULL)
        return;

    if (controller->controller != NULL)
        SDL_GameControllerClose(controller->controller);

    if (controller->mapping != NULL)
        SDL_free((void*) controller->mapping);

    if (controller->name != NULL)
        SDL_free((void*) controller->name);
}
