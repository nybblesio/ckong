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

static bool tile_map_editor_enter(state_context_t* context);
static bool tile_map_editor_update(state_context_t* context);
static bool tile_map_editor_leave(state_context_t* context);

static bool long_introduction_enter(state_context_t* context);
static bool long_introduction_update(state_context_t* context);
static bool long_introduction_leave(state_context_t* context);

static state_t s_tile_map_editor = {
    .state = state_tile_map_editor,
    .enter = tile_map_editor_enter,
    .update = tile_map_editor_update,
    .leave = tile_map_editor_leave
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
// Tile Map Editor State
//
// ----------------------------------------------------------------------------
typedef struct tile_editor_state {
    uint8_t index;
    uint8_t x;
    uint8_t y;
} tile_editor_state_t;

static tile_editor_state_t s_tile_editor;

static void tile_map_select(bool flag) {
    bg_control_block_t* block = video_tile(s_tile_editor.y, s_tile_editor.x);
    if (!flag)
        block->flags &= ~f_bg_select;
    else
        block->flags |= f_bg_select;
    block->flags |= f_bg_changed;
}

static bool tile_map_editor_enter(state_context_t* context) {
    s_tile_editor.index = 0;
    s_tile_editor.x = 0;
    s_tile_editor.y = 0;
    log_message(category_app, "tile map index: %d", s_tile_editor.index);
    video_set_bg(tile_map_index(s_tile_editor.index));
    tile_map_select(true);
    return true;
}

static bool tile_map_editor_update(state_context_t* context) {
    if (game_controller_button(context->controller, button_left_shoulder)) {
        if (s_tile_editor.index > 0) {
            s_tile_editor.index--;
            video_set_bg(tile_map_index(s_tile_editor.index));
            tile_map_select(true);
        }
    }

    if (game_controller_button(context->controller, button_right_shoulder)) {
        if (s_tile_editor.index < TILE_MAP_MAX - 1) {
            s_tile_editor.index++;
            video_set_bg(tile_map_index(s_tile_editor.index));
            tile_map_select(true);
        }
    }

    if (game_controller_button(context->controller, button_a)) {
        bg_control_block_t* block = video_tile(s_tile_editor.y, s_tile_editor.x);
        if (block->palette > 0) {
            block->palette--;
            block->flags |= f_bg_changed;
        }
    }

    if (game_controller_button(context->controller, button_b)) {
        bg_control_block_t* block = video_tile(s_tile_editor.y, s_tile_editor.x);
        if (block->palette < PALETTE_MAX) {
            block->palette++;
            block->flags |= f_bg_changed;
        }
    }

    if (game_controller_button(context->controller, button_x)) {
        bg_control_block_t* block = video_tile(s_tile_editor.y, s_tile_editor.x);
        if (block->tile > 0) {
            block->tile--;
            block->flags |= f_bg_changed;
        }
    }

    if (game_controller_button(context->controller, button_y)) {
        bg_control_block_t* block = video_tile(s_tile_editor.y, s_tile_editor.x);
        if (block->tile < TILE_MAX) {
            block->tile++;
            block->flags |= f_bg_changed;
        }
    }

    if (game_controller_button(context->controller, button_dpad_up)) {
        if (s_tile_editor.y > 0) {
            tile_map_select(false);
            s_tile_editor.y--;
            tile_map_select(true);
        }
    }

    if (game_controller_button(context->controller, button_dpad_down)) {
        if (s_tile_editor.y < TILE_MAP_HEIGHT - 1) {
            tile_map_select(false);
            s_tile_editor.y++;
            tile_map_select(true);
        }
    }

    if (game_controller_button(context->controller, button_dpad_left)) {
        if (s_tile_editor.x > 0) {
            tile_map_select(false);
            s_tile_editor.x--;
            tile_map_select(true);
        }
    }

    if (game_controller_button(context->controller, button_dpad_right)) {
        if (s_tile_editor.x < TILE_MAP_WIDTH - 1) {
            tile_map_select(false);
            s_tile_editor.x++;
            tile_map_select(true);
        }
    }

    return true;
}

static bool tile_map_editor_leave(state_context_t* context) {
    tile_map_save();
    return true;
}

// ----------------------------------------------------------------------------
//
// Long Introduction State
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

static bool long_introduction_enter(state_context_t* context) {
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

    actor_t* donkey_kong = actor(actor_donkey_kong);
    donkey_kong->x = 124;
    donkey_kong->y = 176;
    actor_animation(donkey_kong, anim_donkey_kong_climb_ladder);
    donkey_kong->flags |= f_actor_enabled;

    machine_header_update();
    player1_header_update();
    level_header_update();

    return true;
}

static bool long_introduction_update(state_context_t* context) {
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

static bool long_introduction_leave(state_context_t* context) {
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
            break;
        case state_attract:
            break;
        case state_long_introduction:
            state_ptr = &s_long_introduction;
            break;
        case state_how_high:
            break;
        case state_game:
            break;
        case state_round_won:
            break;
        case state_donkey_kong_flees:
            break;
        case state_tile_map_editor:
            state_ptr = &s_tile_map_editor;
            break;
        default:
            log_error(category_app, "unknown state");
            break;
    }

    s_stack_index--;
    s_state_stack[s_stack_index] = state_ptr;
    s_state_stack[s_stack_index]->enter(context);
}