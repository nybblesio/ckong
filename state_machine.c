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

#include "log.h"
#include "game.h"
#include "tile.h"
#include "level.h"
#include "actor.h"
#include "video.h"
#include "player.h"
#include "machine.h"
#include "palette.h"
#include "state_machine.h"
#include "game_controller.h"

static const color_t s_black = {0x00, 0x00, 0x00, 0xff};
static const color_t s_blue  = {0x00, 0x00, 0xee, 0xff};
static const color_t s_white = {0xff, 0xff, 0xff, 0xff};

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
    uint16_t delay;
} boot_state_t;

static boot_state_t s_boot_state;

static bool boot_enter(state_context_t* context) {
    s_boot_state.tile = 77;
    s_boot_state.delay = 2;
    s_boot_state.palette = 0;

    video_fill_bg(s_boot_state.tile, s_boot_state.palette);
    return true;
}

static bool boot_update(state_context_t* context) {
    if (s_boot_state.delay > 0) {
        s_boot_state.delay--;
    } else {
        s_boot_state.delay = 2;

        if (s_boot_state.palette < 64) {
            s_boot_state.palette++;
        } else {
            state_pop(context);
            state_push(context, state_insert_coin);
            return true;
        }
        video_fill_bg(s_boot_state.tile, s_boot_state.palette);
    }
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
    video_fill_bg(0x0a, 0x0f);
    return true;
}

static bool insert_coin_update(state_context_t* context) {
    if (game_controller_button_pressed(context->controller, button_x)) {
        state_push(context, state_editor);
    }
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
    video_set_bg(tile_map(tile_map_introduction));

    actor_t* oil_barrel = actor(actor_oil_barrel);
    oil_barrel->flags |= f_actor_enabled;

    actor_t* oil_fire = actor(actor_oil_fire);
    oil_fire->flags |= f_actor_enabled;

    actor_t* mario = actor(actor_mario);
    mario->x = 64;
    mario->y = 191;
    mario->data1 = mario_right;
    mario->flags |= f_actor_enabled;

    actor_t* bonus = actor(actor_bonus);
    bonus->x = 100;
    bonus->y = 100;
    bonus->data1 = 4;
    bonus->flags |= f_actor_enabled;
    bonus->animation_callback = bonus_animation_callback;
    actor_animation(bonus, anim_bonus_100);

    actor_t* pauline = actor(actor_pauline);
    pauline->x = 104;
    pauline->y = 43;
    actor_animation(pauline, anim_pauline_stand_right);
    pauline->flags |= f_actor_enabled;

    machine_header_update();
    player1_header_update();
    level_header_update();

    return true;
}

static bool game_screen_1_update(state_context_t* context) {
    // XXX: refactor all of the jumpman handling into player module
    actor_t* mario = actor(actor_mario);

    bool is_climbing = (mario->data1 & mario_climb) != 0
                       || (mario->data1 & mario_climb_end) != 0;

    if (game_controller_button(context->controller, button_dpad_right)
        &&  !is_climbing) {
        if (mario->x < 224)
            mario->x += 2;
        mario->data1 &= ~mario_left;
        mario->data1 |= mario_right | mario_run;
        actor_animation(mario, anim_mario_walk_right);
    } else if (game_controller_button(context->controller, button_dpad_left)
               && !is_climbing) {
        if (mario->x > 16)
            mario->x -= 2;
        mario->data1 &= ~mario_right;
        mario->data1 |= mario_left | mario_run;
        actor_animation(mario, anim_mario_walk_left);
    } else if (game_controller_button(context->controller, button_dpad_up)) {
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

    if (game_controller_button(context->controller, button_a)
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
                dir == 2 ? anim_mario_jump_right : anim_mario_jump_left);
        }
    } else if (is_climbing) {
        actor_animation(mario, anim_mario_climb);
    } else if ((mario->data1 & mario_run) == 0) {
        actor_animation(
            mario,
            dir == 2 ? anim_mario_stand_right : anim_mario_stand_left);
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
static bool how_high_enter(state_context_t* context) {
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
    video_set_bg(tile_map(tile_map_introduction));

    actor_t* donkey_kong = actor(actor_donkey_kong);
    donkey_kong->x = 124;
    donkey_kong->y = 176;
    donkey_kong->flags |= f_actor_enabled;
    actor_animation(donkey_kong, anim_donkey_kong_climb_ladder);

    machine_header_update();
    player1_header_update();
    level_header_update();

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

typedef struct tile_editor_state {
    bool active;
    uint8_t x;
    uint8_t y;
    uint8_t tx;
    uint8_t ty;
    uint8_t px;
    uint8_t py;
    uint16_t tile;
    uint8_t index;
    uint8_t palette;
    tile_editor_action_t action;
} tile_editor_state_t;

static tile_editor_state_t s_tile_editor = {
    .x = 0,
    .y = 0,
    .tx = 0,
    .ty = 0,
    .tile = 0,
    .index = 0,
    .palette = 0,
    .active = false,
    .action = editor_action_none
};

static void tile_map_select(bool flag) {
    bg_control_block_t* block = video_tile(s_tile_editor.y, s_tile_editor.x);
    if (!flag)
        block->flags &= ~f_bg_select;
    else
        block->flags |= f_bg_select;
    block->flags |= f_bg_changed;
}

static bool editor_enter(state_context_t* context) {
    if (s_tile_editor.active)
        return true;

    s_tile_editor.x = 0;
    s_tile_editor.y = 0;
    s_tile_editor.tx = 0;
    s_tile_editor.ty = 0;
    s_tile_editor.px = 0;
    s_tile_editor.py = 0;
    s_tile_editor.tile = 0;
    s_tile_editor.index = 0;
    s_tile_editor.palette = 0;

    log_message(category_app, "tile map index: %d", s_tile_editor.index);
    video_set_bg(tile_map_index(s_tile_editor.index));
    tile_map_select(true);
    s_tile_editor.active = true;

    return true;
}

static bool editor_update(state_context_t* context) {
    video_text(s_white, 2, SCREEN_HEIGHT - 8, "I:%02d", s_tile_editor.index);

    if (game_controller_button_pressed(context->controller, button_left_shoulder)) {
        if (s_tile_editor.index > 0) {
            s_tile_editor.index--;
            video_set_bg(tile_map_index(s_tile_editor.index));
            tile_map_select(true);
        }
    }

    if (game_controller_button_pressed(context->controller, button_right_shoulder)) {
        if (s_tile_editor.index < TILE_MAP_MAX - 1) {
            s_tile_editor.index++;
            video_set_bg(tile_map_index(s_tile_editor.index));
            tile_map_select(true);
        }
    }

    if (game_controller_button_pressed(context->controller, button_x)) {
        state_push(context, state_editor_pick_tile);
        return true;
    }

    if (game_controller_button_pressed(context->controller, button_y)) {
        state_push(context, state_editor_pick_palette);
        return true;
    }

    if (game_controller_button_pressed(context->controller, button_a)) {
        bg_control_block_t* block = video_tile(s_tile_editor.y, s_tile_editor.x);
        if (block != NULL) {
            block->tile = s_tile_editor.tile;
            block->palette = s_tile_editor.palette;
        }
        return true;
    }

    if (game_controller_button_pressed(context->controller, button_guide)) {
        state_push(context, state_editor_menu);
        return true;
    }

    if (game_controller_button_pressed(context->controller, button_back)) {
        s_tile_editor.active = false;
        state_pop(context);
        return true;
    }

    if (game_controller_button_pressed(context->controller, button_dpad_up)) {
        if (s_tile_editor.y > 0) {
            tile_map_select(false);
            s_tile_editor.y--;
            tile_map_select(true);
        }
    }

    if (game_controller_button_pressed(context->controller, button_dpad_down)) {
        if (s_tile_editor.y < TILE_MAP_HEIGHT - 1) {
            tile_map_select(false);
            s_tile_editor.y++;
            tile_map_select(true);
        }
    }

    if (game_controller_button_pressed(context->controller, button_dpad_left)) {
        if (s_tile_editor.x > 0) {
            tile_map_select(false);
            s_tile_editor.x--;
            tile_map_select(true);
        }
    }

    if (game_controller_button_pressed(context->controller, button_dpad_right)) {
        if (s_tile_editor.x < TILE_MAP_WIDTH - 1) {
            tile_map_select(false);
            s_tile_editor.x++;
            tile_map_select(true);
        }
    }

    return true;
}

static bool editor_leave(state_context_t* context) {
    return true;
}

static bool editor_menu_enter(state_context_t* context) {
    return true;
}

static const char* s_menu_options[] = {
    "Back",
    "Save",
    "Fill Row",
    "Fill Column",
    "Fill Map",
    "Exit",
    NULL
};

static bool editor_menu_update(state_context_t* context) {
    if (game_controller_button_pressed(context->controller, button_back)) {
        state_pop(context);
        return true;
    }

    if (game_controller_button_pressed(context->controller, button_a)) {
        state_pop(context);
        return true;
    }

    if (game_controller_button_pressed(context->controller, button_dpad_up)) {
        if (s_tile_editor.action > 0)
            s_tile_editor.action--;
    }

    if (game_controller_button_pressed(context->controller, button_dpad_down)) {
        if (s_tile_editor.action < editor_action_exit)
            s_tile_editor.action++;
    }

    rect_t box = {
        .left = (SCREEN_WIDTH - 128) / 2,
        .top = (SCREEN_HEIGHT - 128) / 2,
        .width = 128,
        .height = 128
    };
    video_fill_rect(s_black, box);
    video_rect(s_white, box);
    video_text(
        s_white,
        box.left + 5,
        box.top + 3,
        "Editor Menu");
    video_hline(s_white, box.left, box.top + 12, box.width);

    uint16_t tx = box.left + 16;
    uint16_t ty = box.top + 20;

    for (uint8_t i = 0;; i++) {
        const char* option = s_menu_options[i];
        if (option == NULL)
            break;
        const bool is_active = i == s_tile_editor.action;
        const color_t* active_color = is_active ? &s_white : &s_blue;
        if (is_active)
            video_text(*active_color, tx - 10, ty, ">");
        video_text(*active_color, tx, ty, option);
        ty += 10;
    }

    return true;
}

static bool editor_menu_leave(state_context_t* context) {
    return true;
}

static bool editor_pick_tile_enter(state_context_t* context) {
    return true;
}

static bool editor_pick_tile_update(state_context_t* context) {
    if (game_controller_button_pressed(context->controller, button_back)) {
        state_pop(context);
        return true;
    }

    if (game_controller_button_pressed(context->controller, button_dpad_up)) {
        if (s_tile_editor.ty > 0)
            s_tile_editor.ty--;
    }

    if (game_controller_button_pressed(context->controller, button_dpad_down)) {
        if (s_tile_editor.ty < 15)
            s_tile_editor.ty++;
    }

    if (game_controller_button_pressed(context->controller, button_dpad_left)) {
        if (s_tile_editor.tx > 0)
            s_tile_editor.tx--;
    }

    if (game_controller_button_pressed(context->controller, button_dpad_right)) {
        if (s_tile_editor.tx < 15)
            s_tile_editor.tx++;
    }

    s_tile_editor.tile = (s_tile_editor.ty * 16) + s_tile_editor.tx;

    rect_t box = {
        .left = (SCREEN_WIDTH - 200) / 2,
        .top = (SCREEN_HEIGHT - 200) / 2,
        .width = 200,
        .height = 200
    };
    video_fill_rect(s_black, box);
    video_rect(s_white, box);
    video_text(
        s_white,
        box.left + 5,
        box.top + 3,
        "Select Tile");
    video_text(
        s_white,
        box.left + 5,
        (box.top + box.height) - 10,
        "Palette:%02d",
        s_tile_editor.palette);
    video_hline(s_white, box.left, box.top + 12, box.width);

    uint16_t tx = box.left + 16;
    uint16_t ty = box.top + 20;

    for (uint16_t tile = 0; tile < TILE_MAX; tile++) {
        uint8_t flags = f_bg_none;
        if (tile == s_tile_editor.tile)
            flags |= f_bg_select;
        video_stamp_tile(tx, ty, tile, s_tile_editor.palette, flags);
        tx += TILE_WIDTH + 2;
        if (tx >= box.width) {
            tx = box.left + 16;
            ty += TILE_HEIGHT + 2;
        }
    }

    return true;
}

static bool editor_pick_tile_leave(state_context_t* context) {
    return true;
}

static bool editor_pick_palette_enter(state_context_t* context) {
    return true;
}

static bool editor_pick_palette_update(state_context_t* context) {
    if (game_controller_button_pressed(context->controller, button_back)) {
        state_pop(context);
        return true;
    }

    if (game_controller_button_pressed(context->controller, button_dpad_up)) {
        if (s_tile_editor.py > 0)
            s_tile_editor.py--;
    }

    if (game_controller_button_pressed(context->controller, button_dpad_down)) {
        if (s_tile_editor.py < 15)
            s_tile_editor.py++;
    }

    if (game_controller_button_pressed(context->controller, button_dpad_left)) {
        if (s_tile_editor.px > 0)
            s_tile_editor.px--;
    }

    if (game_controller_button_pressed(context->controller, button_dpad_right)) {
        if (s_tile_editor.px < 3)
            s_tile_editor.px++;
    }

    s_tile_editor.palette = (s_tile_editor.py * 4) + s_tile_editor.px;

    rect_t box = {
        .left = (SCREEN_WIDTH - 232) / 2,
        .top = (SCREEN_HEIGHT - 232) / 2,
        .width = 232,
        .height = 232
    };
    video_fill_rect(s_black, box);
    video_rect(s_white, box);
    video_text(
        s_white,
        box.left + 5,
        box.top + 3,
        "Select Palette");
    video_hline(s_white, box.left, box.top + 12, box.width);

    uint16_t tx = box.left + 22;
    uint16_t ty = box.top + 16;

    for (uint8_t pal = 0; pal < PALETTE_MAX; pal++) {
        const palette_t* p = palette(pal);
        const color_t* active_color = pal == s_tile_editor.palette ?
            &s_white :
            &s_blue;

        video_text(*active_color, tx - 18, ty + 2, "%02d", pal);

        const rect_t o = {
            .left = tx,
            .top = ty,
            .width = (8 * 4) + 1,
            .height = 10
        };
        video_rect(*active_color, o);

        for (uint8_t i = 0; i < 4; i++) {
            const palette_entry_t* e = &p->entries[i];
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
            tx = box.left + 22;
            ty += 13;
        }
    }

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