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

#include <SDL_scancode.h>
#include "log.h"
#include "game.h"
#include "tile.h"
#include "actor.h"
#include "video.h"
#include "palette.h"
#include "keyboard.h"
#include "state_machine.h"
#include "timer.h"

static const color_t s_green = {0x00, 0xee, 0x00, 0xff};
static const color_t s_black = {0x00, 0x00, 0x00, 0xff};
static const color_t s_blue  = {0x00, 0x00, 0xee, 0xff};
static const color_t s_white = {0xff, 0xff, 0xff, 0xff};
static const color_t s_grey  = {0x2f, 0x2f, 0x2f, 0xff};

static bool boot_enter(state_context_t* context);
static bool boot_update(state_context_t* context);
static bool boot_leave(state_context_t* context);

static bool title_enter(state_context_t* context);
static bool title_update(state_context_t* context);
static bool title_leave(state_context_t* context);

static bool credit_enter(state_context_t* context);
static bool credit_update(state_context_t* context);
static bool credit_leave(state_context_t* context);

static bool how_high_enter(state_context_t* context);
static bool how_high_update(state_context_t* context);
static bool how_high_leave(state_context_t* context);

static bool high_score_enter(state_context_t* context);
static bool high_score_update(state_context_t* context);
static bool high_score_leave(state_context_t* context);

static bool insert_coin_enter(state_context_t* context);
static bool insert_coin_update(state_context_t* context);
static bool insert_coin_leave(state_context_t* context);

static bool game_screen_1_enter(state_context_t* context);
static bool game_screen_1_update(state_context_t* context);
static bool game_screen_1_leave(state_context_t* context);

static bool game_screen_2_enter(state_context_t* context);
static bool game_screen_2_update(state_context_t* context);
static bool game_screen_2_leave(state_context_t* context);

static bool game_screen_3_enter(state_context_t* context);
static bool game_screen_3_update(state_context_t* context);
static bool game_screen_3_leave(state_context_t* context);

static bool game_screen_4_enter(state_context_t* context);
static bool game_screen_4_update(state_context_t* context);
static bool game_screen_4_leave(state_context_t* context);

static bool editor_enter(state_context_t* context);
static bool editor_update(state_context_t* context);
static bool editor_leave(state_context_t* context);

static bool editor_menu_enter(state_context_t* context);
static bool editor_menu_update(state_context_t* context);
static bool editor_menu_leave(state_context_t* context);

static bool editor_pick_tile_enter(state_context_t* context);
static bool editor_pick_tile_update(state_context_t* context);
static bool editor_pick_tile_leave(state_context_t* context);

static bool editor_pick_palette_enter(state_context_t* context);
static bool editor_pick_palette_update(state_context_t* context);
static bool editor_pick_palette_leave(state_context_t* context);

static bool long_introduction_enter(state_context_t* context);
static bool long_introduction_update(state_context_t* context);
static bool long_introduction_leave(state_context_t* context);

static state_t s_boot = {
    .state = state_boot,
    .enter = boot_enter,
    .update = boot_update,
    .leave = boot_leave
};

static state_t s_title = {
    .state = state_title,
    .enter = title_enter,
    .update = title_update,
    .leave = title_leave
};

static state_t s_credit = {
    .state = state_credit,
    .enter = credit_enter,
    .update = credit_update,
    .leave = credit_leave
};

static state_t s_how_high = {
    .state = state_how_high,
    .enter = how_high_enter,
    .update = how_high_update,
    .leave = how_high_leave
};

static state_t s_high_score = {
    .state = state_how_high,
    .enter = high_score_enter,
    .update = high_score_update,
    .leave = high_score_leave
};

static state_t s_insert_coin = {
    .state = state_insert_coin,
    .enter = insert_coin_enter,
    .update = insert_coin_update,
    .leave = insert_coin_leave
};

static state_t s_game_screen_1 = {
    .state = state_game_screen_1,
    .enter = game_screen_1_enter,
    .update = game_screen_1_update,
    .leave = game_screen_1_leave
};

static state_t s_game_screen_2 = {
    .state = state_game_screen_2,
    .enter = game_screen_2_enter,
    .update = game_screen_2_update,
    .leave = game_screen_2_leave
};

static state_t s_game_screen_3 = {
    .state = state_game_screen_3,
    .enter = game_screen_3_enter,
    .update = game_screen_3_update,
    .leave = game_screen_3_leave
};

static state_t s_game_screen_4 = {
    .state = state_game_screen_4,
    .enter = game_screen_4_enter,
    .update = game_screen_4_update,
    .leave = game_screen_4_leave
};

static state_t s_editor = {
    .state = state_editor,
    .enter = editor_enter,
    .update = editor_update,
    .leave = editor_leave
};

static state_t s_editor_menu = {
    .state = state_editor_menu,
    .enter = editor_menu_enter,
    .update = editor_menu_update,
    .leave = editor_menu_leave
};

static state_t s_editor_pick_tile = {
    .state = state_editor_pick_tile,
    .enter = editor_pick_tile_enter,
    .update = editor_pick_tile_update,
    .leave = editor_pick_tile_leave
};

static state_t s_editor_pick_palette = {
    .state = state_editor_pick_palette,
    .enter = editor_pick_palette_enter,
    .update = editor_pick_palette_update,
    .leave = editor_pick_palette_leave
};

static state_t s_long_introduction = {
    .state = state_long_introduction,
    .enter = long_introduction_enter,
    .update = long_introduction_update,
    .leave = long_introduction_leave
};

static state_t* s_state_stack[32];

static uint8_t s_stack_index = 32;

// ----------------------------------------------------------------------------
//
// Helpers
//
// ----------------------------------------------------------------------------
static void copy_bg(const tile_map_t* tile_map) {
    uint32_t i = 0;
    for (uint16_t y = 0; y < TILE_MAP_HEIGHT; y++) {
        for (uint16_t x = 0; x < TILE_MAP_WIDTH; x++) {
            bg_control_block_t* block = video_tile(y, x);
            tile_map_entry_t* entry = (tile_map_entry_t*) &tile_map->data[i];
            entry->flags = 0;
            entry->tile = block->tile;
            entry->palette = block->palette;

            if ((block->flags & f_bg_vflip) == f_bg_vflip)
                entry->flags |= f_bg_vflip;

            if ((block->flags & f_bg_hflip) == f_bg_hflip)
                entry->flags |= f_bg_hflip;

            ++i;
        }
    }
}

// ----------------------------------------------------------------------------
//
// Callbacks
//
// ----------------------------------------------------------------------------
static bool bonus_animation_callback(actor_t* actor) {
    if (actor->data1 > 0) {
        actor->data1--;
        return true;
    }
    actor->flags &= ~f_actor_enabled;
    return false;
}

// ----------------------------------------------------------------------------
//
// Boot State
//
// ----------------------------------------------------------------------------
typedef struct {
    uint16_t tile;
    uint8_t palette;
    timer_t* timer;
} boot_state_t;

static boot_state_t s_boot_state;

static bool boot_timer_callback(timer_t* timer, uint32_t ticks) {
    if (s_boot_state.palette < 64) {
        s_boot_state.palette += 2;
        return true;
    } else {
        state_context_t* context = (state_context_t*) timer->user;

        state_pop(context);
        state_push(context, state_insert_coin);

        s_boot_state.timer = NULL;

        return false;
    }
}

static bool boot_enter(state_context_t* context) {
    s_boot_state.tile = 77;
    s_boot_state.palette = 0;
    s_boot_state.timer = timer_start(
        context->ticks,
        17,
        boot_timer_callback,
        context);
    return true;
}

static bool boot_update(state_context_t* context) {
    video_bg_fill(s_boot_state.tile, s_boot_state.palette);
    return true;
}

static bool boot_leave(state_context_t* context) {
    return true;
}

// ----------------------------------------------------------------------------
//
// Credit State
//
// ----------------------------------------------------------------------------
static bool credit_enter(state_context_t* context) {
    return true;
}

static bool credit_update(state_context_t* context) {
    return true;
}

static bool credit_leave(state_context_t* context) {
    return true;
}

// ----------------------------------------------------------------------------
//
// Insert Coin State
//
// ----------------------------------------------------------------------------
static bool insert_coin_enter(state_context_t* context) {
    video_bg_set(tile_map(tile_map_insert_coin));
    return true;
}

static bool insert_coin_update(state_context_t* context) {
    machine_t* machine = context->machine;
    if (joystick_button_pressed(context->joystick, button_x)) {
        state_push(context, state_editor);
        return true;
    }

    if (key_pressed(SDL_SCANCODE_1)) {
        if (machine->credits[0] < 0xff) {
            machine->credits[0]++;
        }
    }

    if (key_pressed(SDL_SCANCODE_2)) {
        if (machine->credits[1] < 0xff) {
            machine->credits[1]++;
        }
    }

    if (joystick_button_pressed(context->joystick, button_start)
    &&  context->machine->credits > 0) {
        state_pop(context);
        state_push(context, state_how_high);
        return true;
    }

    video_bg_str(
        31,
        27,
        3,
        true,
        "%02d",
        machine->credits[0] + machine->credits[1]);

    return true;
}

static bool insert_coin_leave(state_context_t* context) {
    return true;
}

// ----------------------------------------------------------------------------
//
// Title State
//
// ----------------------------------------------------------------------------
static bool title_enter(state_context_t* context) {
    return true;
}

static bool title_update(state_context_t* context) {
    return true;
}

static bool title_leave(state_context_t* context) {
    return true;
}

// ----------------------------------------------------------------------------
//
// Game Screen 1 State
//
// ----------------------------------------------------------------------------
static bool game_screen_1_enter(state_context_t* context) {
    video_bg_set(tile_map(tile_map_game_screen_1));

    actor_t* oil_barrel = actor(actor_oil_barrel);
    oil_barrel->flags |= f_actor_enabled;

    actor_t* oil_fire = actor(actor_oil_fire);
    oil_fire->flags |= f_actor_enabled;

    actor_t* mario = actor(actor_mario);
    mario->x = 32;
    mario->y = 232;
    mario->data1 = mario_right;
    mario->flags |= f_actor_enabled;

    actor_t* bonus = actor(actor_bonus);
    bonus->x = 100;
    bonus->y = 100;
    bonus->data1 = 4;
    bonus->flags |= f_actor_enabled;
    bonus->animation_callback = bonus_animation_callback;
    actor_animation(bonus, anim_bonus_100, context->ticks);

    actor_t* pauline = actor(actor_pauline);
    pauline->x = 104;
    pauline->y = 43;
    actor_animation(pauline, anim_pauline_stand_right, context->ticks);
    pauline->flags |= f_actor_enabled;

    machine_header_update();
    player1_header_update(context->ticks);
    level_header_update(context->ticks);

    return true;
}

static bool game_screen_1_update(state_context_t* context) {
    // XXX: refactor all of the jumpman handling into player module
    actor_t* mario = actor(actor_mario);

    bool is_climbing = (mario->data1 & mario_climb) != 0
                       || (mario->data1 & mario_climb_end) != 0;

    if (joystick_button(context->joystick, button_dpad_right)
        &&  !is_climbing) {
        if (mario->x < 224)
            mario->x += 2;
        mario->data1 &= ~mario_left;
        mario->data1 |= mario_right | mario_run;
        actor_animation(mario, anim_mario_walk_right, context->ticks);
    } else if (joystick_button(context->joystick, button_dpad_left)
               && !is_climbing) {
        if (mario->x > 16)
            mario->x -= 2;
        mario->data1 &= ~mario_right;
        mario->data1 |= mario_left | mario_run;
        actor_animation(mario, anim_mario_walk_left, context->ticks);
    } else if (joystick_button(context->joystick, button_dpad_up)) {
        // is mario over a ladder?
        //
        // first, we compute mario's position within the background tile map
        uint8_t tx = (uint8_t) ((mario->x + 8) / 8);
        uint8_t ty = (uint8_t) ((mario->y + 8) / 8);

        // grab the block
        bg_control_block_t* block = video_tile(ty, tx);

        log_message(category_app, "block = %d", block->tile);

        // is it a ladder piece?
        if (block->tile == 0xc0) {
            mario->x = (int16_t) (tx * 8 - 4);
            mario->y -= 2;
            mario->data1 |= mario_climb;
        }
    } else {
        mario->data1 &= ~mario_run;
    }

    if (joystick_button(context->joystick, button_a)
    &&  (mario->data1 & mario_jump) == 0) {
        mario->data1 |= mario_jump;
        mario->data2 = 20;
    }

    int8_t dir = 1;
    if ((mario->data1 & mario_left) != 0) {
        dir = 1;
    } else if ((mario->data1 & mario_right) != 0) {
        dir = 2;
    }

    if ((mario->data1 & mario_jump) != 0) {
        if (mario->data2 > 10)
            mario->y--;
        else
            mario->y++;
        mario->data2--;
        if (mario->data2 == 0)
            mario->data1 &= ~mario_jump;
        else {
            actor_animation(
                mario,
                dir == 2 ? anim_mario_jump_right : anim_mario_jump_left,
                context->ticks);
        }
    } else if (is_climbing) {
        actor_animation(mario, anim_mario_climb, context->ticks);
    } else if ((mario->data1 & mario_run) == 0) {
        actor_animation(
            mario,
            dir == 2 ? anim_mario_stand_right : anim_mario_stand_left,
            context->ticks);
    }

    return true;
}

static bool game_screen_1_leave(state_context_t* context) {
    return true;
}

// ----------------------------------------------------------------------------
//
// Game Screen 2 State
//
// ----------------------------------------------------------------------------
static bool game_screen_2_enter(state_context_t* context) {
    return true;
}

static bool game_screen_2_update(state_context_t* context) {
    return true;
}

static bool game_screen_2_leave(state_context_t* context) {
    return true;
}

// ----------------------------------------------------------------------------
//
// Game Screen 3 State
//
// ----------------------------------------------------------------------------
static bool game_screen_3_enter(state_context_t* context) {
    return true;
}

static bool game_screen_3_update(state_context_t* context) {
    return true;
}

static bool game_screen_3_leave(state_context_t* context) {
    return true;
}

// ----------------------------------------------------------------------------
//
// Game Screen 4 State
//
// ----------------------------------------------------------------------------
static bool game_screen_4_enter(state_context_t* context) {
    return true;
}

static bool game_screen_4_update(state_context_t* context) {
    return true;
}

static bool game_screen_4_leave(state_context_t* context) {
    return true;
}

// ----------------------------------------------------------------------------
//
// How High? State
//
// ----------------------------------------------------------------------------
typedef struct {
    int8_t level;
    uint8_t stage;
    uint8_t row;
} level_elevation_t;

static timer_t* s_how_high_duration = NULL;

static level_elevation_t s_level_elevations[] = {
    {1, 1, 25}, // level 1, stage 1
    {1, 2, 21}, // level 1, stage 2

    {2, 1, 25}, // level 2, stage 1
    {2, 2, 21}, // level 2, stage 2
    {2, 3, 17}, // level 2, stage 3

    {3, 1, 25}, // level 3, stage 1
    {3, 2, 21}, // level 3, stage 2
    {3, 3, 17}, // level 3, stage 3
    {3, 4, 13}, // level 3, stage 4

    {4, 1, 25}, // level 4, stage 1
    {4, 2, 21}, // level 4, stage 2
    {4, 3, 17}, // level 4, stage 3
    {4, 4, 13}, // level 4, stage 4
    {4, 5,  9}, // level 4, stage 5

    {-1, 0, 0}  // end
};

static const level_elevation_t* elevation(const player_t* player) {
    for (uint8_t i = 0; ; i++) {
        const level_elevation_t* e = &s_level_elevations[i];
        if (e->level == -1)
            break;
        if (e->level == player->level
        &&  e->stage == player->stage) {
            return e;
        }
    }
    return NULL;
}

static bool how_high_timer_callback(timer_t* timer, uint32_t ticks) {
    s_how_high_duration = NULL;

    state_context_t* context = (state_context_t*) timer->user;

    state_pop(context);
    // XXX: the next state is based on level/stage
    state_push(context, state_game_screen_1);

    return false;
}

static bool how_high_enter(state_context_t* context) {
    video_bg_set(tile_map(tile_map_how_high));

    player_t* player = context->player;
    player->level = 4;
    player->stage = 5;

    const level_elevation_t* e = elevation(player);
    if (e != NULL) {
        for (uint8_t y = 5; y < e->row; y++) {
            for (uint8_t x = 7; x < 20; x++) {
                bg_control_block_t* block = video_tile(y, x);
                block->flags &= ~f_bg_enabled | f_bg_changed;
            }
        }
    }

    s_how_high_duration = timer_start(
        context->ticks,
        SECONDS(3),
        how_high_timer_callback,
        context);

    return true;
}

static bool how_high_update(state_context_t* context) {
    return true;
}

static bool how_high_leave(state_context_t* context) {
    return true;
}

// ----------------------------------------------------------------------------
//
// High Score State
//
// ----------------------------------------------------------------------------
static bool high_score_enter(state_context_t* context) {
    return true;
}

static bool high_score_update(state_context_t* context) {
    return true;
}

static bool high_score_leave(state_context_t* context) {
    return true;
}

// ----------------------------------------------------------------------------
//
// Long Introduction State
//
// ----------------------------------------------------------------------------
static bool long_introduction_enter(state_context_t* context) {
    video_bg_set(tile_map(tile_map_introduction));

    actor_t* donkey_kong = actor(actor_donkey_kong);
    donkey_kong->x = 124;
    donkey_kong->y = 176;
    donkey_kong->flags |= f_actor_enabled;
    actor_animation(
        donkey_kong,
        anim_donkey_kong_climb_ladder,
        context->ticks);

    machine_header_update();
    player1_header_update(context->ticks);
    level_header_update(context->ticks);

    return true;
}

static bool long_introduction_update(state_context_t* context) {
    actor_t* donkey_kong = actor(actor_donkey_kong);

    if (donkey_kong->y > 30)
        donkey_kong->y--;

    return true;
}

static bool long_introduction_leave(state_context_t* context) {
    return true;
}

// ----------------------------------------------------------------------------
//
// Tile Map Editor State
//
// ----------------------------------------------------------------------------
typedef enum {
    editor_action_none,
    editor_action_save,
    editor_action_fill_row,
    editor_action_fill_column,
    editor_action_fill_map,
    editor_action_exit
} tile_editor_action_t;

typedef struct {
    uint8_t x, y;
    uint8_t value;
} grid_value_t;

typedef struct {
    uint8_t x1, y1;
    uint8_t x2, y2;
} copy_range_t;

typedef struct tile_editor_state {
    uint8_t x;
    uint8_t y;
    bool active;
    uint8_t index;
    bool text_entry;
    char message[32];
    grid_value_t tile;
    bool select_range;
    bool cursor_visible;
    grid_value_t palette;
    uint16_t cursor_frames;
    timer_t* message_timer;
    tile_editor_action_t action;
    bg_control_block_t* copy_buffer;
} tile_editor_state_t;

typedef struct {
    int32_t scancode;
    uint8_t ascii;
} scancode_to_ascii_t;

static const scancode_to_ascii_t s_ascii_keys[] = {
    {SDL_SCANCODE_0,        '0'},
    {SDL_SCANCODE_1,        '1'},
    {SDL_SCANCODE_2,        '2'},
    {SDL_SCANCODE_3,        '3'},
    {SDL_SCANCODE_4,        '4'},
    {SDL_SCANCODE_5,        '5'},
    {SDL_SCANCODE_6,        '6'},
    {SDL_SCANCODE_7,        '7'},
    {SDL_SCANCODE_8,        '8'},
    {SDL_SCANCODE_9,        '9'},
    {SDL_SCANCODE_A,        'A'},
    {SDL_SCANCODE_B,        'B'},
    {SDL_SCANCODE_C,        'C'},
    {SDL_SCANCODE_D,        'D'},
    {SDL_SCANCODE_E,        'E'},
    {SDL_SCANCODE_F,        'F'},
    {SDL_SCANCODE_G,        'G'},
    {SDL_SCANCODE_H,        'H'},
    {SDL_SCANCODE_I,        'I'},
    {SDL_SCANCODE_J,        'J'},
    {SDL_SCANCODE_K,        'K'},
    {SDL_SCANCODE_L,        'L'},
    {SDL_SCANCODE_M,        'M'},
    {SDL_SCANCODE_N,        'N'},
    {SDL_SCANCODE_O,        'O'},
    {SDL_SCANCODE_P,        'P'},
    {SDL_SCANCODE_Q,        'Q'},
    {SDL_SCANCODE_R,        'R'},
    {SDL_SCANCODE_S,        'S'},
    {SDL_SCANCODE_T,        'T'},
    {SDL_SCANCODE_U,        'U'},
    {SDL_SCANCODE_V,        'V'},
    {SDL_SCANCODE_W,        'W'},
    {SDL_SCANCODE_X,        'X'},
    {SDL_SCANCODE_Y,        'Y'},
    {SDL_SCANCODE_Z,        'Z'},
    {SDL_SCANCODE_EQUALS,   '='},
    {SDL_SCANCODE_MINUS,    '-'},
    {SDL_SCANCODE_SPACE,    ' '},
    {-1,                    0}
};

static const char* s_menu_options[] = {
    "Back",
    "Save",
    "Fill Row",
    "Fill Column",
    "Fill Map",
    "Exit",
    NULL
};

static grid_value_t s_tile_undo;
static copy_range_t s_copy_range;
static grid_value_t s_palette_undo;
static tile_editor_state_t s_tile_editor;

static bool message_timer_callback(timer_t* timer, uint32_t ticks) {
    s_tile_editor.message_timer = NULL;
    return false;
}

static void draw_cursor(uint32_t y, uint32_t x) {
    if (s_tile_editor.cursor_frames > 0) {
        s_tile_editor.cursor_frames--;
    } else {
        s_tile_editor.cursor_frames = 30;
        s_tile_editor.cursor_visible = !s_tile_editor.cursor_visible;
    }

    if (s_tile_editor.cursor_visible) {
        rect_t cursor_rect = {
            .left = x,
            .top =  y,
            .width = TILE_WIDTH,
            .height = TILE_HEIGHT
        };
        video_fill_rect(s_green, cursor_rect);
    }
}

static void draw_footer(const bg_control_block_t* block) {
    if (s_tile_editor.message_timer != NULL) {
        video_text(
            s_green,
            SCREEN_HEIGHT - 20,
            2,
            s_tile_editor.message);
    }

    video_text(
        s_white,
        SCREEN_HEIGHT - 10,
        1,
        "I:%01X "
        "%02X/%02X "
        "%02X/%02X "
        "%c|%c "
        "%s",
        s_tile_editor.index,
        s_tile_editor.y,
        s_tile_editor.x,
        block->palette,
        block->tile,
        (block->flags & f_bg_hflip) == f_bg_hflip ? 'H' : '-',
        (block->flags & f_bg_vflip) == f_bg_vflip ? 'V' : '-',
        s_tile_editor.text_entry ? "ASCII" : "TILE");
}

static void show_message(uint32_t ticks, uint32_t duration, const char* fmt, ...) {
    s_tile_editor.message_timer = timer_start(
        ticks,
        duration,
        message_timer_callback,
        NULL);

    va_list list;
    va_start(list, fmt);
    vsnprintf(s_tile_editor.message, 32, fmt, list);
    va_end(list);
}

static bool editor_enter(state_context_t* context) {
    if (s_tile_editor.active) {
        switch (s_tile_editor.action) {
            case editor_action_save: {
                copy_bg(tile_map(s_tile_editor.index));
                tile_map_save();
                show_message(context->ticks, SECONDS(2), "ckong.dat saved");
                break;
            }
            case editor_action_exit: {
                s_tile_editor.active = false;
                state_pop(context);
                break;
            }
            case editor_action_fill_map: {
                video_bg_fill(s_tile_editor.tile.value, s_tile_editor.palette.value);
                show_message(context->ticks, SECONDS(2), "tile map filled");
                break;
            }
            case editor_action_fill_row: {
                for (uint16_t x = 0; x < TILE_MAP_WIDTH; x++) {
                    bg_control_block_t* block = video_tile(s_tile_editor.y, x);
                    if (block != NULL) {
                        block->flags |= f_bg_changed;
                        block->tile = s_tile_editor.tile.value;
                        block->palette = s_tile_editor.palette.value;
                    }
                }
                show_message(context->ticks, SECONDS(2), "tile map row filled");
                break;
            }
            case editor_action_fill_column: {
                for (uint16_t y = 0; y < TILE_MAP_HEIGHT; y++) {
                    bg_control_block_t* block = video_tile(y, s_tile_editor.x);
                    if (block != NULL) {
                        block->flags |= f_bg_changed;
                        block->tile = s_tile_editor.tile.value;
                        block->palette = s_tile_editor.palette.value;
                    }
                }
                show_message(context->ticks, SECONDS(2), "tile map column filled");
                break;
            }
            default: {
                break;
            }
        }

        s_tile_editor.action = editor_action_none;

        return true;
    }

    s_tile_editor.x = 0;
    s_tile_editor.y = 0;
    s_tile_editor.index = 0;
    s_tile_editor.tile.x = 0;
    s_tile_editor.tile.y = 0;
    s_tile_editor.tile.value = 0;
    s_tile_editor.palette.x = 0;
    s_tile_editor.palette.y = 0;
    s_tile_editor.palette.value = 0;
    s_tile_editor.message_timer = NULL;

    s_tile_editor.text_entry = false;

    s_tile_editor.cursor_frames = 30;
    s_tile_editor.cursor_visible = true;

    s_tile_editor.copy_buffer = NULL;

    log_message(category_app, "tile map index: %d", s_tile_editor.index);
    video_bg_set(tile_map(s_tile_editor.index));
    s_tile_editor.active = true;

    return true;
}

static bool editor_update(state_context_t* context) {
    bg_control_block_t* block = video_tile(s_tile_editor.y, s_tile_editor.x);

    if (joystick_button_pressed(context->joystick, button_x)) {
        state_push(context, state_editor_pick_tile);
        return true;
    }

    if (key_pressed(SDL_SCANCODE_F2)) {
        s_tile_editor.text_entry = !s_tile_editor.text_entry;
    }

    if (key_state(SDL_SCANCODE_RCTRL) && !s_tile_editor.select_range) {
        if (key_state(SDL_SCANCODE_C)) {
            if (s_tile_editor.copy_buffer != NULL) {
                free(s_tile_editor.copy_buffer);
                s_tile_editor.copy_buffer = NULL;
            }

            s_copy_range.x1 = s_tile_editor.x;
            s_copy_range.y1 = s_tile_editor.y;
            s_tile_editor.select_range = true;

            return true;
        }

        if (key_state(SDL_SCANCODE_V)
        &&  s_tile_editor.copy_buffer != NULL) {
            const uint32_t width = (s_copy_range.x2 - s_copy_range.x1) + 1;
            const uint32_t height = (s_copy_range.y2 - s_copy_range.y1) + 1;

            uint32_t i = 0;
            uint8_t tx = s_tile_editor.x;
            uint8_t ty = s_tile_editor.y;
            for (uint8_t y = 0; y < height; y++) {
                for (uint8_t x = 0; x < width; x++) {
                    bg_control_block_t* target_block = video_tile(ty, tx++);
                    *target_block = s_tile_editor.copy_buffer[i++];
                    target_block->flags |= f_bg_changed;
                }
                ++ty;
                tx = s_tile_editor.x;
            }

            free(s_tile_editor.copy_buffer);
            s_tile_editor.copy_buffer = NULL;
            s_copy_range.x1 = 0;
            s_copy_range.y1 = 0;
            s_copy_range.x2 = 0;
            s_copy_range.y2 = 0;

            return true;
        }
    }

    if (key_pressed(SDL_SCANCODE_RETURN)
    &&  s_tile_editor.select_range) {
        s_copy_range.x2 = s_tile_editor.x;
        s_copy_range.y2 = s_tile_editor.y;

        const uint32_t width = (s_copy_range.x2 - s_copy_range.x1) + 1;
        const uint32_t height = (s_copy_range.y2 - s_copy_range.y1) + 1;

        s_tile_editor.copy_buffer = malloc((height * width) * sizeof(bg_control_block_t));
        uint32_t i = 0;
        for (uint8_t y = s_copy_range.y1; y <= s_copy_range.y2; y++) {
            for (uint8_t x = s_copy_range.x1; x <= s_copy_range.x2; x++) {
                s_tile_editor.copy_buffer[i++] = *video_tile(y, x);
            }
        }

        s_tile_editor.select_range = false;
    }

    if (key_pressed(SDL_SCANCODE_DELETE)) {
        bg_control_block_t last_block = *video_tile(s_tile_editor.y, TILE_MAP_WIDTH - 1);

        for (uint8_t x = s_tile_editor.x; x < TILE_MAP_WIDTH - 1; x++) {
            bg_control_block_t* current_block = video_tile(s_tile_editor.y, x);
            bg_control_block_t* next_block = video_tile(s_tile_editor.y, x + 1);
            *current_block = *next_block;
            current_block->flags |= f_bg_changed;
        }

        *video_tile(s_tile_editor.y, TILE_MAP_WIDTH - 1) = last_block;
    }

    if (key_pressed(SDL_SCANCODE_INSERT)) {
        bg_control_block_t start_block = *video_tile(s_tile_editor.y, s_tile_editor.x);

        for (uint8_t x = TILE_MAP_WIDTH - 2; x > s_tile_editor.x; x--) {
            bg_control_block_t* current_block = video_tile(s_tile_editor.y, x + 1);
            bg_control_block_t* prev_block = video_tile(s_tile_editor.y, x);
            *current_block = *prev_block;
            current_block->flags |= f_bg_changed;
        }

        *video_tile(s_tile_editor.y, s_tile_editor.x) = start_block;
    }

    if (s_tile_editor.text_entry) {
        if (key_pressed(SDL_SCANCODE_BACKSPACE)) {
            block->tile = 0x0a;
            block->flags |= f_bg_changed;
            if (s_tile_editor.x > 0)
                s_tile_editor.x--;
        } else {
            for (uint32_t i = 0;; i++) {
                const scancode_to_ascii_t* entry = &s_ascii_keys[i];
                if (entry->scancode == -1)
                    break;

                if (key_pressed(entry->scancode)) {
                    if (block != NULL) {
                        const uint8_t c = entry->ascii;
                        if (c == 0x20) {
                            block->tile = 0x0a;
                        } else if (c == '=') {
                            block->tile = 52;
                        } else if (c > 128) {
                            block->tile = c;
                        } else {
                            block->tile = (uint16_t) (c - 48);
                        }
                        block->flags |= f_bg_changed;
                        block->palette = s_tile_editor.palette.value;
                    }
                    if (s_tile_editor.x < TILE_MAP_WIDTH - 1) {
                        s_tile_editor.x++;
                    } else {
                        s_tile_editor.x = 0;
                        if (s_tile_editor.y < TILE_MAP_HEIGHT - 1)
                            s_tile_editor.y++;
                    }
                    break;
                }
            }
        }
    }

    if (key_pressed(SDL_SCANCODE_HOME)) {
        s_tile_editor.x = 0;
    }

    if (key_pressed(SDL_SCANCODE_END)) {
        s_tile_editor.x = TILE_MAP_WIDTH - 1;
    }

    if (key_pressed(SDL_SCANCODE_PAGEUP)) {
        s_tile_editor.y = 0;
    }

    if (key_pressed(SDL_SCANCODE_PAGEDOWN)) {
        s_tile_editor.y = TILE_MAP_HEIGHT - 1;
    }

    if (key_pressed(SDL_SCANCODE_F3)) {
        bool is_set = block->flags & f_bg_hflip;
        if (is_set) {
            block->flags &= ~f_bg_hflip;
        } else {
            block->flags |= f_bg_hflip;
        }
        block->flags |= f_bg_changed;
    }

    if (key_pressed(SDL_SCANCODE_F4)) {
        bool is_set = block->flags & f_bg_vflip;
        if (is_set) {
            block->flags &= ~f_bg_vflip;
        } else {
            block->flags |= f_bg_vflip;
        }
        block->flags |= f_bg_changed;
    }

    if (joystick_button_pressed(context->joystick, button_y)
    && !s_tile_editor.text_entry) {
        state_push(context, state_editor_pick_palette);
        return true;
    }

    if (joystick_button_pressed(context->joystick, button_a)
    && !s_tile_editor.text_entry
    && !s_tile_editor.select_range) {
        bg_control_block_t* block = video_tile(s_tile_editor.y, s_tile_editor.x);
        if (block != NULL) {
            block->flags |= f_bg_changed;
            block->tile = s_tile_editor.tile.value;
            block->palette = s_tile_editor.palette.value;
        }
        return true;
    }

    if (joystick_button_pressed(context->joystick, button_guide)) {
        state_push(context, state_editor_menu);
        return true;
    }

    if (joystick_button_pressed(context->joystick, button_back)) {
        if (!s_tile_editor.text_entry) {
            copy_bg(tile_map(s_tile_editor.index));
            s_tile_editor.active = false;
            state_pop(context);
        }
        return true;
    }

    if (joystick_button_pressed(context->joystick, button_dpad_up)) {
        const uint8_t min_y = s_tile_editor.select_range ? s_copy_range.y1 : 0;
        if (s_tile_editor.y > min_y)
            s_tile_editor.y--;
    }

    if (joystick_button_pressed(context->joystick, button_dpad_down)) {
        if (s_tile_editor.y < TILE_MAP_HEIGHT - 1)
            s_tile_editor.y++;
    }

    if (joystick_button_pressed(context->joystick, button_dpad_left)) {
        const uint8_t min_x = s_tile_editor.select_range ? s_copy_range.x1 : 0;
        if (s_tile_editor.x > min_x)
            s_tile_editor.x--;
    }

    if (joystick_button_pressed(context->joystick, button_dpad_right)) {
        if (s_tile_editor.x < TILE_MAP_WIDTH - 1)
            s_tile_editor.x++;
    }

    if (joystick_button_pressed(context->joystick, button_left_shoulder)) {
        if (s_tile_editor.index > 0) {
            copy_bg(tile_map(s_tile_editor.index));
            s_tile_editor.index--;
            video_bg_set(tile_map(s_tile_editor.index));
        }
    }

    if (joystick_button_pressed(context->joystick, button_right_shoulder)) {
        if (s_tile_editor.index < TILE_MAP_MAX - 1) {
            copy_bg(tile_map(s_tile_editor.index));
            s_tile_editor.index++;
            video_bg_set(tile_map(s_tile_editor.index));
        }
    }

    const uint32_t cursor_y = s_tile_editor.y * TILE_HEIGHT;
    const uint32_t cursor_x = s_tile_editor.x * TILE_WIDTH;

    draw_cursor(cursor_y, cursor_x);

    if (s_tile_editor.select_range) {
        rect_t rect = {
            .left = s_copy_range.x1 * TILE_WIDTH,
            .top = s_copy_range.y1 * TILE_HEIGHT,
            .width = (((s_tile_editor.x - s_copy_range.x1) + 1) * TILE_WIDTH) - 1,
            .height = (((s_tile_editor.y - s_copy_range.y1) + 1) * TILE_HEIGHT) - 1
        };
        video_rect(s_green, rect);
    }

    draw_footer(block);

    return true;
}

static bool editor_leave(state_context_t* context) {
    return true;
}

static bool editor_menu_enter(state_context_t* context) {
    return true;
}

static bool editor_menu_update(state_context_t* context) {
    if (joystick_button_pressed(context->joystick, button_a)) {
        state_pop(context);
        return true;
    }

    if (joystick_button_pressed(context->joystick, button_back)) {
        state_pop(context);
        return true;
    }

    if (joystick_button_pressed(context->joystick, button_dpad_up)) {
        if (s_tile_editor.action > 0)
            s_tile_editor.action--;
    }

    if (joystick_button_pressed(context->joystick, button_dpad_down)) {
        if (s_tile_editor.action < editor_action_exit)
            s_tile_editor.action++;
    }

    rect_t box = {
        .left = (SCREEN_WIDTH - 128) / 2,
        .top = (SCREEN_HEIGHT - 128) / 2,
        .width = 128,
        .height = 128
    };
    video_fill_rect(s_grey, box);
    video_rect(s_white, box);
    video_text(
        s_white,
        box.top + 3,
        box.left + 5,
        "Editor Menu");
    video_hline(s_white, box.top + 12, box.left, box.width);

    uint16_t tx = box.left + 16;
    uint16_t ty = box.top + 20;

    for (uint8_t i = 0;; i++) {
        const char* option = s_menu_options[i];
        if (option == NULL)
            break;
        const bool is_active = i == s_tile_editor.action;
        const color_t* active_color = is_active ? &s_white : &s_blue;
        if (is_active)
            video_text(*active_color, ty, tx - 10, ">");
        video_text(*active_color, ty, tx, option);
        ty += 10;
    }

    return true;
}

static bool editor_menu_leave(state_context_t* context) {
    return true;
}

static bool editor_pick_tile_enter(state_context_t* context) {
    s_tile_undo = s_tile_editor.tile;
    return true;
}

static bool editor_pick_tile_update(state_context_t* context) {
    grid_value_t* tile = &s_tile_editor.tile;

    if (joystick_button_pressed(context->joystick, button_a)) {
        state_pop(context);
        return true;
    }

    if (joystick_button_pressed(context->joystick, button_back)) {
        *tile = s_tile_undo;
        state_pop(context);
        return true;
    }

    if (joystick_button_pressed(context->joystick, button_dpad_up)) {
        if (tile->y > 0)
            tile->y--;
    }

    if (joystick_button_pressed(context->joystick, button_dpad_down)) {
        if (tile->y < 15)
            tile->y++;

        const uint16_t max_x = s_tile_editor.tile.y == 15 ? 11 : 15;
        if (tile->x > max_x)
            tile->x = max_x;
    }

    if (joystick_button_pressed(context->joystick, button_dpad_left)) {
        if (tile->x > 0)
            tile->x--;
    }

    if (joystick_button_pressed(context->joystick, button_dpad_right)) {
        const uint16_t max_x = s_tile_editor.tile.y == 15 ? 11 : 15;
        if (tile->x < max_x)
            tile->x++;
    }

    if (joystick_button_pressed(context->joystick, button_left_shoulder)) {
        if (s_tile_editor.palette.value > 0)
            s_tile_editor.palette.value--;
    }

    if (joystick_button_pressed(context->joystick, button_right_shoulder)) {
        if (s_tile_editor.palette.value < PALETTE_MAX - 1)
            s_tile_editor.palette.value++;
    }

    tile->value = (tile->y * 16) + tile->x;

    rect_t box = {
        .left = (SCREEN_WIDTH - 200) / 2,
        .top = (SCREEN_HEIGHT - 200) / 2,
        .width = 200,
        .height = 200
    };
    video_fill_rect(s_grey, box);
    video_rect(s_white, box);
    video_text(
        s_white,
        box.top + 3,
        box.left + 5,
        "Select Tile");
    video_text(
        s_white,
        (box.top + box.height) - 10,
        box.left + 5,
        "P:%02X T:%02X",
        s_tile_editor.palette.value,
        s_tile_editor.tile.value);
    video_hline(s_white, box.top + 12, box.left, box.width);

    const uint16_t left_edge = box.left + 16;
    const uint16_t top_edge = box.top + 20;

    uint16_t tx = left_edge;
    uint16_t ty = top_edge;

    for (uint16_t t = 0; t < TILE_MAX; t++) {
        video_stamp_tile(ty, tx, t, s_tile_editor.palette.value, f_bg_none);
        tx += TILE_WIDTH + 2;
        if (tx >= box.width) {
            tx = left_edge;
            ty += TILE_HEIGHT + 2;
        }
    }

    draw_cursor(
        top_edge + (tile->y * (TILE_HEIGHT + 2)),
        left_edge + (tile->x * (TILE_WIDTH + 2)));

    return true;
}

static bool editor_pick_tile_leave(state_context_t* context) {
    return true;
}

static bool editor_pick_palette_enter(state_context_t* context) {
    s_palette_undo = s_tile_editor.palette;
    return true;
}

static bool editor_pick_palette_update(state_context_t* context) {
    grid_value_t* pal = &s_tile_editor.palette;

    if (joystick_button_pressed(context->joystick, button_a)) {
        state_pop(context);
        return true;
    }

    if (joystick_button_pressed(context->joystick, button_back)) {
        *pal = s_palette_undo;
        state_pop(context);
        return true;
    }

    if (joystick_button_pressed(context->joystick, button_dpad_up)) {
        if (pal->y > 0)
            pal->y--;
    }

    if (joystick_button_pressed(context->joystick, button_dpad_down)) {
        if (pal->y < 15)
            pal->y++;
    }

    if (joystick_button_pressed(context->joystick, button_dpad_left)) {
        if (pal->x > 0)
            pal->x--;
    }

    if (joystick_button_pressed(context->joystick, button_dpad_right)) {
        if (pal->x < 3)
            pal->x++;
    }

    pal->value = (pal->y * 4) + pal->x;

    rect_t box = {
        .left = (SCREEN_WIDTH - 232) / 2,
        .top = (SCREEN_HEIGHT - 232) / 2,
        .width = 232,
        .height = 232
    };
    video_fill_rect(s_grey, box);
    video_rect(s_white, box);
    video_text(
        s_white,
        box.top + 3,
        box.left + 5,
        "Select Palette");
    video_hline(s_white, box.top + 12, box.left, box.width);

    const uint16_t top_edge = box.top + 16;
    const uint16_t left_edge = box.left + 22;

    uint16_t tx = left_edge;
    uint16_t ty = top_edge;

    for (uint8_t i = 0; i < PALETTE_MAX; i++) {
        const palette_t* p = palette(i);

        video_text(s_white, ty + 2, tx - 18, "%02X", i);

        const rect_t o = {
            .left = tx,
            .top = ty,
            .width = (8 * 4) + 1,
            .height = 10
        };
        video_rect(s_white, o);

        for (uint8_t j = 0; j < 4; j++) {
            const palette_entry_t* e = &p->entries[j];
            const rect_t r = {
                .left = tx + 1,
                .top = ty + 2,
                .width = 7,
                .height = 7
            };
            const color_t c = {
                .r = e->red,
                .g = e->green,
                .b = e->blue,
                .a = 0xff
            };
            video_fill_rect(c, r);
            tx += 8;
        }

        tx += 24;
        if (tx >= (box.width - 16)) {
            tx = left_edge;
            ty += 13;
        }
    }

    draw_cursor(
        top_edge + 2 + (pal->y * 13),
        (left_edge - 18) + (pal->x * 56));

    return true;
}

static bool editor_pick_palette_leave(state_context_t* context) {
    return true;
}

// ----------------------------------------------------------------------------
//
// State Machine Public Interface
//
// ----------------------------------------------------------------------------

void state_pop(state_context_t* context) {
    if (s_stack_index == 32)
        return;

    s_state_stack[s_stack_index]->leave(context);
    s_stack_index++;
    if (s_stack_index < 32)
        s_state_stack[s_stack_index]->enter(context);
}

void state_update(state_context_t* context) {
    if (s_stack_index == 32)
        return;
    s_state_stack[s_stack_index]->update(context);
}

void state_push(state_context_t* context, states_t state) {
    if (s_stack_index < 32) {
        s_state_stack[s_stack_index]->leave(context);
    }

    state_t* state_ptr = NULL;
    switch (state) {
        case state_boot:
            state_ptr = &s_boot;
            break;
        case state_title:
            state_ptr = &s_title;
            break;
        case state_credit:
            state_ptr = &s_credit;
            break;
        case state_editor:
            state_ptr = &s_editor;
            break;
        case state_how_high:
            state_ptr = &s_how_high;
            break;
        case state_high_score:
            state_ptr = &s_high_score;
            break;
        case state_insert_coin:
            state_ptr = &s_insert_coin;
            break;
        case state_editor_menu:
            state_ptr = &s_editor_menu;
            break;
        case state_game_screen_1:
            state_ptr = &s_game_screen_1;
            break;
        case state_game_screen_2:
            state_ptr = &s_game_screen_2;
            break;
        case state_game_screen_3:
            state_ptr = &s_game_screen_3;
            break;
        case state_game_screen_4:
            state_ptr = &s_game_screen_4;
            break;
        case state_editor_pick_tile:
            state_ptr = &s_editor_pick_tile;
            break;
        case state_long_introduction:
            state_ptr = &s_long_introduction;
            break;
        case state_editor_pick_palette:
            state_ptr = &s_editor_pick_palette;
            break;
        default:
            log_error(category_app, "unknown state");
            break;
    }

    s_stack_index--;
    s_state_stack[s_stack_index] = state_ptr;
    s_state_stack[s_stack_index]->enter(context);
}