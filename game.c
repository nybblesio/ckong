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
#include "video.h"
#include "window.h"

game_context_t* game_context_new() {
    game_context_t* context = malloc(sizeof(game_context_t));
    context->messages = linked_list_new_node();
    context->controller = NULL;
    context->valid = false;
    return context;
}

bool game_run(game_context_t* context) {
    bool quit = false;
    SDL_Event e;

    SDL_Surface* vg_surface = video_surface();

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

    int32_t number_of_joysticks = joystick_count();
    for (int32_t i = 0; i < number_of_joysticks; ++i) {
        if (joystick_is_controller(i)) {
            context->controller = game_controller_open(i);
            break;
        }
    }

    video_init();
    video_set_bg(tile_map(long_introduction));

    spr_control_block_t* mario = video_sprite(0);
    mario->y = 80;
    mario->x = 32;
    mario->tile = 0;
    mario->palette = 2;
    mario->flags |= f_spr_enabled | f_spr_hflip;

    context->valid = true;

    return true;
}

void game_shutdown(game_context_t* context) {
    IMG_Quit();

    if (context == NULL)
        return;

    game_controller_close(context->controller);

    if (context->window.texture != NULL)
        SDL_DestroyTexture(context->window.texture);

    if (context->window.renderer != NULL)
        SDL_DestroyRenderer(context->window.renderer);

    if (context->window.window != NULL)
        SDL_DestroyWindow(context->window.window);

    if (context->window.messages != NULL)
        linked_list_free(context->window.messages);

    if (context->messages != NULL)
        linked_list_free(context->messages);

    video_shutdown();
}
