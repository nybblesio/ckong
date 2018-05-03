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
#include <SDL2/SDL_gamecontroller.h>
#include "game_controller.h"

bool game_controller_button(
        game_controller_t* controller,
        game_controller_button_t button) {
    return SDL_GameControllerGetButton(
        controller->controller,
        (SDL_GameControllerButton) button) != 0;
}

int32_t joystick_count(void) {
    return SDL_NumJoysticks();
}

bool joystick_is_controller(int32_t index) {
    return SDL_IsGameController(index) == SDL_TRUE;
}

game_controller_t* game_controller_open(int32_t index) {
    game_controller_t* controller = malloc(sizeof(game_controller_t));
    controller->index = index;
    controller->controller = SDL_GameControllerOpen(index);
    controller->name = SDL_GameControllerNameForIndex(index);
    controller->mapping = SDL_GameControllerMappingForIndex(index);
    return controller;
}

void game_controller_close(game_controller_t* controller) {
    if (controller == NULL || controller->controller == NULL)
        return;

    SDL_GameControllerClose(controller->controller);

    if (controller->mapping != NULL)
        SDL_free((void*) controller->mapping);

    if (controller->name != NULL)
        SDL_free((void*) controller->name);
}
