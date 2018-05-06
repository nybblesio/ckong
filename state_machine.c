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

#include "state_machine.h"
#include "actor.h"
#include "game.h"
#include "video.h"

static bool long_introduction_enter(state_context_t* context);
static bool long_introduction_update(state_context_t* context);
static bool long_introduction_leave(state_context_t* context);

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
// Long Introduction State
//
// ----------------------------------------------------------------------------
static bool long_introduction_enter(state_context_t* context) {
    video_set_bg(tile_map(long_introduction));

    actor_t* mario = actor(actor_mario);
    mario->x = 32;
    mario->y = 224;
    mario->data1 = mario_right;

    return true;
}

static bool long_introduction_update(state_context_t* context) {
    actor_t* mario = actor(actor_mario);

    if (game_controller_button(context->controller, button_dpad_right)) {
        if (mario->x < 224)
            mario->x += 2;
        mario->data1 &= ~mario_left;
        mario->data1 |= mario_right | mario_run;
        actor_animation(mario, anim_mario_walk_right);
    } else if (game_controller_button(context->controller, button_dpad_left)) {
        if (mario->x > 16)
            mario->x -= 2;
        mario->data1 &= ~mario_right;
        mario->data1 |= mario_left | mario_run;
        actor_animation(mario, anim_mario_walk_left);
    } else {
        mario->data1 &= ~mario_run;
    }

    if (game_controller_button(context->controller, button_a)
        &&  (mario->data1 & mario_jump) == 0) {
        mario->data1 |= mario_jump;
        mario->data2 = 20;
    }

    // XXX: temporary test code
    if (game_controller_button(context->controller, button_dpad_up)) {
        mario->y -= 2;
    }

    if (game_controller_button(context->controller, button_dpad_down)) {
        mario->y += 2;
    }
    // XXX: temporary test code

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
    }

    s_stack_index--;
    s_state_stack[s_stack_index] = state_ptr;
    s_state_stack[s_stack_index]->enter(context);
}