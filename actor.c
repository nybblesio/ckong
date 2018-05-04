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

#include <SDL2/SDL_timer.h>
#include "actor.h"
#include "video.h"

static animation_t s_mario_stand_left = {
    .frame_count = 1,
    .frames = {
        {
            .delay = 0,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 0,
                    .palette = 2
                }
            }
        },
    }
};

static animation_t s_mario_stand_right = {
    .frame_count = 1,
    .frames = {
        {
            .delay = 0,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 0,
                    .palette = 2,
                    .flags = f_spr_hflip
                }
            }
        },
    }
};

static animation_t s_mario_jump_left = {
    .frame_count = 1,
    .frames = {
        {
            .delay = 0,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 14,
                    .palette = 2
                }
            }
        },
    }
};

static animation_t s_mario_jump_right = {
    .frame_count = 1,
    .frames = {
        {
            .delay = 0,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 14,
                    .palette = 2,
                    .flags = f_spr_hflip
                }
            }
        },
    }
};

static animation_t s_mario_walk_left = {
    .frame_count = 3,
    .frames = {
        {
            .delay = 66,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 0,
                    .palette = 2,
                }
            }
        },
        {
            .delay = 66,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 1,
                    .palette = 2,
                }
            }
        },
        {
            .delay = 66,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 2,
                    .palette = 2,
                }
            }
        },
    }
};

static animation_t s_mario_walk_right = {
    .frame_count = 3,
    .frames = {
        {
            .delay = 66,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 0,
                    .palette = 2,
                    .flags = f_spr_hflip
                }
            }
        },
        {
            .delay = 66,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 1,
                    .palette = 2,
                    .flags = f_spr_hflip
                }
            }
        },
        {
            .delay = 66,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 2,
                    .palette = 2,
                    .flags = f_spr_hflip
                }
            }
        },
    }
};

static actor_t s_mario = {
    .x = 0,
    .y = 0,
    .frame = 0,
    .next_tick = 0,
    .animation = NULL,
    .flags = f_actor_none,
    .animation_type = anim_none
};

static actor_t* s_actors[] = {
    &s_mario,
};

static const uint32_t actors_max = 1;

void actor_update(void) {
    video_reset_sprites();

    uint8_t sprite_number = 0;
    for (uint32_t i = 0; i < actors_max; i++) {
        actor_t* actor = s_actors[i];

        if (actor->animation_type == anim_none
        ||  actor->animation == NULL)
            continue;

        animation_frame_t* frame = &actor->animation->frames[actor->frame];
        for (uint32_t j = 0; j < frame->tile_count; j++) {
            animation_frame_tile_t* frame_tile = &frame->tiles[j];
            spr_control_block_t* block = video_sprite(sprite_number++);
            block->x = (uint16_t) (actor->x + frame_tile->x_offset);
            block->y = (uint16_t) (actor->y + frame_tile->y_offset);
            block->tile = frame_tile->tile;
            block->palette = frame_tile->palette;
            block->flags |= frame_tile->flags | f_spr_enabled;
        }

        if (actor->animation->frame_count > 1) {
            uint32_t current_ticks = SDL_GetTicks();
            if (current_ticks >= actor->next_tick) {
                actor->frame++;
                if (actor->frame >= actor->animation->frame_count)
                    actor->frame = 0;
                actor->next_tick = current_ticks + actor->animation->frames[actor->frame].delay;
            }
        }
    }
}

actor_t* actor(actors_t actor) {
    switch (actor) {
        case actor_mario:
            return &s_mario;
    }
    return NULL;
}

void actor_animation(actor_t* actor, animations_t animation) {
    if (actor->animation_type == animation)
        return;

    actor->frame = 0;
    actor->animation_type = animation;

    switch (animation) {
        case anim_mario_walk_left:
            actor->animation = &s_mario_walk_left;
            break;
        case anim_mario_walk_right:
            actor->animation = &s_mario_walk_right;
            break;
        case anim_mario_stand_left:
            actor->animation = &s_mario_stand_left;
            break;
        case anim_mario_stand_right:
            actor->animation = &s_mario_stand_right;
            break;
        case anim_mario_jump_left:
            actor->animation = &s_mario_jump_left;
            break;
        case anim_mario_jump_right:
            actor->animation = &s_mario_jump_right;
            break;
        default:
            break;
    }

    actor->next_tick = SDL_GetTicks() + actor->animation->frames[0].delay;
}
