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

#include <SDL.h>
#include "str.h"
#include "log.h"
#include "game.h"
#include "actor.h"
#include "video.h"
#include "window.h"
#include "player.h"
#include "machine.h"
#include "state_machine.h"
#include "game_controller.h"

static state_context_t s_state_context = {
    .player = NULL,
    .machine = NULL,
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
    context->valid = false;
    context->controller = NULL;
    context->messages = linked_list_new_node();
    return context;
}

bool game_run(game_context_t* context) {
    SDL_Surface* vg_surface = video_surface();

    uint16_t frame_count = 0;
    uint32_t last_time = SDL_GetTicks();
    uint32_t last_fps_time = last_time;

    while (!should_quit()) {
        uint32_t frame_start_ticks = SDL_GetTicks();

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
        uint32_t frame_duration = SDL_GetTicks() - frame_start_ticks;

        uint32_t fps_dt = last_time - last_fps_time;
        if (fps_dt >= 1000) {
            log_message(category_app, "FPS: %d", frame_count);

            frame_count = 0;
            last_fps_time = last_time;
        }

        ++frame_count;

        if (frame_duration < MS_PER_FRAME) {
            SDL_Delay(MS_PER_FRAME - frame_duration);
        }

        last_time = SDL_GetTicks();
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

    log_message(category_app, "Create application window.");
    context->window = window_create();
    if (!context->window.valid) {
        return false;
    }

    machine_init();
    tile_map_init();
    tile_map_load();
    video_init();

    context->controller = game_controller_open();
    s_state_context.controller = context->controller;
    s_state_context.machine = machine();
    s_state_context.player = player1();

    state_push(&s_state_context, state_boot);

    context->valid = true;

    return true;
}

void game_shutdown(game_context_t* context) {
    log_message(category_app, "pop last state from stack.");
    state_pop(&s_state_context);

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
