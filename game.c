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

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.h"
#include "window.h"

game_context_t* game_context_new() {
    game_context_t* context = malloc(sizeof(game_context_t));
    context->messages = linked_list_new_node();
    context->valid = false;
    return context;
}

bool game_run(game_context_t* context) {
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: {
                        quit = true;
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }
        SDL_SetRenderDrawColor(
            context->window.renderer,
            0x7f,
            0x69,
            0x49,
            0xff);
        SDL_RenderClear(context->window.renderer);
        SDL_RenderPresent(context->window.renderer);
    }

    return true;
}

bool game_init(game_context_t* context) {
    context->valid = false;
    context->messages = linked_list_new_node();

    int sdl_result = SDL_Init(SDL_INIT_EVERYTHING);
    if (sdl_result < 0) {
        context->messages->data = str_clone("SDL failed to initialize");
        return false;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        context->messages->data = str_clone("SDL_image failed to initialize");
        return false;
    }

    context->window = window_create();
    if (!context->window.valid) {
        return false;
    }

    context->valid = true;

    return true;
}

void game_shutdown(game_context_t* context) {
    IMG_Quit();

    if (context == NULL)
        return;

    if (context->window.renderer != NULL)
        SDL_DestroyRenderer(context->window.renderer);

    if (context->window.window != NULL)
        SDL_DestroyWindow(context->window.window);

    if (context->window.messages != NULL)
        linked_list_free(context->window.messages);

    if (context->messages != NULL)
        linked_list_free(context->messages);
}
