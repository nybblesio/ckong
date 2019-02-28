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
#include <ini.h>
#include <unistd.h>
#include "str.h"
#include "log.h"
#include "game.h"
#include "timer.h"
#include "actor.h"
#include "video.h"
#include "window.h"
#include "player.h"
#include "machine.h"
#include "joystick.h"
#include "state_machine.h"

static config_t s_config = {
    .win_x = -1,
    .win_y = -1
};

static bool s_show_fps = true;

static state_context_t s_state_context = {
    .player = NULL,
    .machine = NULL,
    .level = NULL,
    .joystick = NULL
};

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

static int config_handler(
        void* user,
        const char* section,
        const char* name,
        const char* value) {
    config_t* config = (config_t*) user;

    if (MATCH("window", "x")) {
        config->win_x = atoi(value);
    } else if (MATCH("window", "y")) {
        config->win_y = atoi(value);
    } else {
        return 0;
    }

    return 1;
}

bool game_config_load() {
    if (access("ckong.ini", F_OK) == -1) {
        log_warn(category_app, "ckong.ini file is missing; not loading.");
        return true;
    }
    return ini_parse("ckong.ini", config_handler, &s_config) == 0;
}

const config_t* game_config() {
    return &s_config;
}

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
                case SDLK_BACKQUOTE: {
                    s_show_fps = !s_show_fps;
                    break;
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
    context->joystick = NULL;
    context->messages = ll_new_node();
    return context;
}

bool game_run(game_context_t* context) {
    const color_t white = {.r = 0xff, .g = 0xff, .b = 0xff, .a = 0xff};

    uint16_t fps = 0;
    uint16_t frame_count = 0;
    uint32_t last_time = SDL_GetTicks();
    uint32_t last_fps_time = last_time;

    while (!should_quit()) {
        uint32_t frame_start_ticks = SDL_GetTicks();

        timer_update(frame_start_ticks);

        s_state_context.ticks = frame_start_ticks;
        state_update(&s_state_context);

        actor_update(frame_start_ticks);

        if (s_show_fps)
            video_text(white, 2, 2, "FPS: %d", fps);

        video_update(&context->window, frame_start_ticks);

        uint32_t frame_duration = SDL_GetTicks() - frame_start_ticks;

        uint32_t fps_dt = last_time - last_fps_time;
        if (fps_dt >= 1000) {
            fps = frame_count;
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
    timer_init();

    game_config_load();

    log_message(category_app, "SDL_Init all the things.");
    int sdl_result = SDL_Init(SDL_INIT_EVERYTHING);
    if (sdl_result < 0) {
        context->messages->data = str_clone("SDL failed to initialize");
        return false;
    }

    log_message(category_app, "Create application window.");
    context->window = window_create(s_config.win_y, s_config.win_x);
    if (!context->window.valid) {
        return false;
    }

    machine_init();
    machine_load();

    tile_map_init();
    tile_map_load();

    video_init(context->window.renderer);

    log_message(category_app, "connect to joystick.");
    context->joystick = joystick_open();
    s_state_context.joystick = context->joystick;
    s_state_context.machine = machine();
    s_state_context.player = player1();

    state_push(&s_state_context, state_long_introduction);

    context->valid = true;

    return true;
}

void game_shutdown(game_context_t* context) {
    log_message(category_app, "save machine state.");
    machine_save();

    log_message(category_app, "pop last state from stack.");
    state_pop(&s_state_context);

    log_message(category_app, "save game config.");
    if (!game_config_save(context)) {
        log_error(category_app, "unable to save game config.");
    }

    if (context == NULL)
        return;

    log_message(category_app, "close & free joystick_t.");
    joystick_close(context->joystick);

    video_shutdown();

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
        ll_free(context->window.messages);

    if (context->messages != NULL)
        ll_free(context->messages);
}

bool game_config_save(const game_context_t* context) {
    int x, y;
    SDL_GetWindowPosition(context->window.window, &x, &y);

    FILE* file = fopen("ckong.ini", "wt");
    if (file != NULL) {
        fprintf(file, "; CKong config file\n\n");
        fprintf(file, "[window]\n");
        fprintf(file, "x = %d\n", x);
        fprintf(file, "y = %d\n", y);
        return true;
    }

    return false;
}
