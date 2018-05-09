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
#include "log.h"
#include "game.h"
#include "actor.h"
#include "video.h"
#include "window.h"
#include "state_machine.h"

static player_t s_player1 = {
    .lives = 3,
    .level = 0,
    .score = 0,
};

static state_context_t s_state_context = {
    .player = &s_player1,
    .level = NULL,
    .controller = NULL
};

static bool should_quit(void) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            return true;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE: {
                    return true;
                }
                default: {
                    break;
                }
            }
        }
    }
    return false;
}

game_context_t* game_context_new() {
    log_message(category_app, "create empty game_context_t");
    game_context_t* context = malloc(sizeof(game_context_t));
    context->messages = linked_list_new_node();
    context->controller = NULL;
    context->valid = false;
    return context;
}

bool game_run(game_context_t* context) {
    SDL_Surface* vg_surface = video_surface();

    while (!should_quit()) {
        state_update(&s_state_context);

        actor_update();

        video_update();

        SDL_UpdateTexture(
            context->window.texture,
            NULL,
            vg_surface->pixels,
            vg_surface->pitch);

        SDL_RenderCopy(
            context->window.renderer,
            context->window.texture,
            NULL,
            NULL);

        SDL_RenderPresent(context->window.renderer);
    }

    return true;
}

bool game_init(game_context_t* context) {
    log_message(category_app, "SDL_Init all the things.");
    int sdl_result = SDL_Init(SDL_INIT_EVERYTHING);
    if (sdl_result < 0) {
        context->messages->data = str_clone("SDL failed to initialize");
        return false;
    }

    log_message(category_app, "IMG_Init for IMG_INIT_PNG.");
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        context->messages->data = str_clone("SDL_image failed to initialize");
        return false;
    }

    log_message(category_app, "Create application window.");
    context->window = window_create();
    if (!context->window.valid) {
        return false;
    }

    context->controller = game_controller_open();
    s_state_context.controller = context->controller;

    tile_map_init();
    video_init();

    state_push(&s_state_context, state_tile_map_editor);

    context->valid = true;

    return true;
}

void game_shutdown(game_context_t* context) {
    log_message(category_app, "pop last state from stack.");
    state_pop(&s_state_context);

    log_message(category_app, "IMG_Quit.");
    IMG_Quit();

    if (context == NULL)
        return;

    log_message(category_app, "close & free game_controller_t.");
    game_controller_close(context->controller);

    log_message(category_app, "destroy streaming texture.");
    if (context->window.texture != NULL)
        SDL_DestroyTexture(context->window.texture);

    log_message(category_app, "destroy renderer.");
    if (context->window.renderer != NULL)
        SDL_DestroyRenderer(context->window.renderer);

    log_message(category_app, "destroy window.");
    if (context->window.window != NULL)
        SDL_DestroyWindow(context->window.window);

    log_message(category_app, "free messages list.");
    if (context->window.messages != NULL)
        linked_list_free(context->window.messages);

    if (context->messages != NULL)
        linked_list_free(context->messages);

    video_shutdown();
}
