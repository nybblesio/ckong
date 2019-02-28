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

#include <SDL_timer.h>
#include "actor.h"
#include "video.h"

static animation_t s_bonus_100_anim = {
    .frame_count = 4,
    .frames = {
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 123,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = -2,
                    .y_offset = -2,
                    .tile = 123,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = -4,
                    .tile = 123,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 2,
                    .y_offset = -6,
                    .tile = 123,
                    .palette = 1
                }
            }
        },
    }
};

static animation_t s_bonus_200_anim = {
    .frame_count = 4,
    .frames = {
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 124,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = -2,
                    .y_offset = -2,
                    .tile = 124,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = -4,
                    .tile = 124,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 2,
                    .y_offset = -6,
                    .tile = 124,
                    .palette = 1
                }
            }
        },
    }
};

static animation_t s_bonus_300_anim = {
    .frame_count = 4,
    .frames = {
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 125,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = -2,
                    .y_offset = -2,
                    .tile = 125,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = -4,
                    .tile = 125,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 2,
                    .y_offset = -6,
                    .tile = 125,
                    .palette = 1
                }
            }
        },
    }
};

static animation_t s_bonus_500_anim = {
    .frame_count = 4,
    .frames = {
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 126,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = -2,
                    .y_offset = -2,
                    .tile = 126,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = -4,
                    .tile = 126,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 2,
                    .y_offset = -6,
                    .tile = 126,
                    .palette = 1
                }
            }
        },
    }
};

static animation_t s_bonus_800_anim = {
    .frame_count = 4,
    .frames = {
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 127,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = -2,
                    .y_offset = -2,
                    .tile = 127,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = -4,
                    .tile = 127,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 2,
                    .y_offset = -6,
                    .tile = 127,
                    .palette = 1
                }
            }
        },
    }
};

static animation_t s_oil_barrel_anim = {
    .frame_count = 1,
    .frames = {
        {
            .delay = 0,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 73,
                    .palette = 12
                }
            }
        },
    }
};

static animation_t s_oil_fire_anim = {
    .frame_count = 4,
    .frames = {
        {
            .delay = 80,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 64,
                    .palette = 1
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 65,
                    .palette = 1
                }
            }
        },
        {
            .delay = 70,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 66,
                    .palette = 1
                }
            }
        },
        {
            .delay = 120,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 67,
                    .palette = 1,
                }
            }
        },
    }
};

static animation_t s_pauline_stand_right = {
    .frame_count = 1,
    .frames = {
        {
            .delay = 0,
            .tile_count = 2,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = -16,
                    .tile = 16,
                    .palette = 9,
                },
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 17,
                    .palette = 10,
                }
            }
        },
    }
};

static animation_t s_pauline_shuffle_right = {
    .frame_count = 2,
    .frames = {
        {
            .delay = 133,
            .tile_count = 2,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = -16,
                    .tile = 16,
                    .palette = 9,
                },
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 17,
                    .palette = 10,
                }
            }
        },
        {
            .delay = 133,
            .tile_count = 2,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = -16,
                    .tile = 16,
                    .palette = 9,
                },
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 18,
                    .palette = 10,
                }
            }
        },
    }
};

static animation_t s_pauline_shuffle_left = {
    .frame_count = 2,
    .frames = {
        {
            .delay = 133,
            .tile_count = 2,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = -16,
                    .tile = 16,
                    .palette = 9,
                    .flags = f_spr_hflip
                },
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 17,
                    .palette = 10,
                    .flags = f_spr_hflip
                }
            }
        },
        {
            .delay = 133,
            .tile_count = 2,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = -16,
                    .tile = 16,
                    .palette = 9,
                    .flags = f_spr_hflip
                },
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 18,
                    .palette = 10,
                    .flags = f_spr_hflip
                }
            }
        },
    }
};

static animation_t s_pauline_stand_left = {
    .frame_count = 1,
    .frames = {
        {
            .delay = 0,
            .tile_count = 2,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = -16,
                    .tile = 16,
                    .palette = 9,
                    .flags = f_spr_hflip
                },
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 17,
                    .palette = 10,
                    .flags = f_spr_hflip
                }
            }
        },
    }
};

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

static animation_t s_mario_climb = {
    .frame_count = 2,
    .frames = {
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 3,
                    .palette = 2,
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 3,
                    .palette = 2,
                    .flags = f_spr_hflip
                }
            }
        },
    }
};

static animation_t s_mario_climb_end = {
    .frame_count = 3,
    .frames = {
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 4,
                    .palette = 2,
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 5,
                    .palette = 2,
                }
            }
        },
        {
            .delay = 100,
            .tile_count = 1,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 6,
                    .palette = 2,
                }
            }
        },
    }
};

static animation_t s_donkey_kong_title_pose = {
    .frame_count = 1,
    .frames = {
        {
            .delay = 0,
            .tile_count = 3,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 50,
                    .palette = 8,
                },
                {
                    .x_offset = 16,
                    .y_offset = 0,
                    .tile = 32,
                    .palette = 8,
                },
                {
                    .x_offset = 32,
                    .y_offset = 0,
                    .tile = 40,
                    .palette = 8,
                },
            }
        },
    }
};

static animation_t s_donkey_kong_climb = {
    .frame_count = 2,
    .frames = {
        {
            .delay = 250,
            .tile_count = 7,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 50,
                    .palette = 8,
                },
                {
                    .x_offset = 16,
                    .y_offset = 0,
                    .tile = 51,
                    .palette = 8,
                },
                {
                    .x_offset = -3,
                    .y_offset = -3,
                    .tile = 52,
                    .palette = 8,
                },
                {
                    .x_offset = 24,
                    .y_offset = 2,
                    .tile = 49,
                    .palette = 8,
                },
                {
                    .x_offset = 0,
                    .y_offset = 16,
                    .tile = 54,
                    .palette = 8,
                },
                {
                    .x_offset = 16,
                    .y_offset = 16,
                    .tile = 55,
                    .palette = 8,
                },
                {
                    .x_offset = 24,
                    .y_offset = 10,
                    .tile = 19,
                    .palette = 10,
                },
            }
        },
        {
            .delay = 250,
            .tile_count = 7,
            .tiles = {
                {
                    .x_offset = 0,
                    .y_offset = 0,
                    .tile = 50,
                    .palette = 8,
                },
                {
                    .x_offset = 16,
                    .y_offset = 0,
                    .tile = 51,
                    .palette = 8,
                },
                {
                    .x_offset = -3,
                    .y_offset = -3,
                    .tile = 53,
                    .palette = 8,
                    .flags = f_spr_hflip
                },
                {
                    .x_offset = 24,
                    .y_offset = 2,
                    .tile = 49,
                    .palette = 8,
                },
                {
                    .x_offset = 0,
                    .y_offset = 16,
                    .tile = 55,
                    .palette = 8,
                    .flags = f_spr_hflip
                },
                {
                    .x_offset = 16,
                    .y_offset = 16,
                    .tile = 54,
                    .palette = 8,
                    .flags = f_spr_hflip
                },
                {
                    .x_offset = 24,
                    .y_offset = 10,
                    .tile = 19,
                    .palette = 10,
                },
            }
        },
    }
};

static actor_t s_mario_actor = {
    .x = 0,
    .y = 0,
    .frame = 0,
    .next_tick = 0,
    .animation = NULL,
    .flags = f_actor_none,
    .animation_type = anim_none,
    .animation_callback = NULL
};

static actor_t s_pauline_actor = {
    .x = 0,
    .y = 0,
    .frame = 0,
    .next_tick = 0,
    .animation = NULL,
    .flags = f_actor_none,
    .animation_type = anim_none,
    .animation_callback = NULL
};

static actor_t s_bonus_actor = {
    .x = 0,
    .y = 0,
    .frame = 0,
    .next_tick = 0,
    .animation = NULL,
    .flags = f_actor_none,
    .animation_type = anim_none,
    .animation_callback = NULL
};

static actor_t s_oil_barrel_actor = {
    .x = 12,
    .y = 232,
    .frame = 0,
    .next_tick = 0,
    .flags = f_actor_none,
    .animation = &s_oil_barrel_anim,
    .animation_type = anim_oil_barrel,
    .animation_callback = NULL
};

static actor_t s_oil_fire_actor = {
    .x = 12,
    .y = 216,
    .frame = 0,
    .next_tick = 0,
    .flags = f_actor_none,
    .animation = &s_oil_fire_anim,
    .animation_type = anim_oil_fire,
    .animation_callback = NULL
};

static actor_t s_donkey_kong_actor = {
    .x = 0,
    .y = 0,
    .frame = 0,
    .next_tick = 0,
    .animation = NULL,
    .flags = f_actor_none,
    .animation_callback = NULL,
    .animation_type = anim_none,
};

static actor_t s_heart_actor = {
    .x = 0,
    .y = 0,
    .frame = 0,
    .next_tick = 0,
    .animation = NULL,
    .flags = f_actor_none,
    .animation_callback = NULL,
    .animation_type = anim_none,
};

static actor_t* s_actors[] = {
    &s_mario_actor,
    &s_oil_barrel_actor,
    &s_oil_fire_actor,
    &s_bonus_actor,
    &s_pauline_actor,
    &s_donkey_kong_actor,
    &s_heart_actor,
    NULL
};

void actor_reset() {
    video_reset_sprites();

    for (uint32_t i = 0;; i++) {
        actor_t* actor = s_actors[i];
        if (actor == NULL)
            break;
        actor->flags = f_actor_none;
    }
}

void actor_update(uint32_t ticks) {
    video_reset_sprites();

    uint8_t sprite_number = 0;
    for (uint32_t i = 0; ; i++) {
        actor_t* actor = s_actors[i];
        if (actor == NULL)
            break;

        if ((actor->flags & f_actor_enabled) == 0)
            continue;

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
            if (ticks >= actor->next_tick) {
                if (actor->frame < actor->animation->frame_count - 1)
                    actor->frame++;
                else {
                    actor->frame = 0;
                    if (actor->animation_callback != NULL) {
                        if (!actor->animation_callback(actor)) {
                            actor->animation_type = anim_none;
                            actor->animation = NULL;
                            actor->next_tick = 0;
                        }
                    }
                }
                if (actor->animation != NULL)
                    actor->next_tick = ticks + actor->animation->frames[actor->frame].delay;
            }
        }
    }
}

actor_t* actor(actors_t actor) {
    switch (actor) {
        case actor_mario:
            return &s_mario_actor;
        case actor_pauline:
            return &s_pauline_actor;
        case actor_oil_barrel:
            return &s_oil_barrel_actor;
        case actor_oil_fire:
            return &s_oil_fire_actor;
        case actor_bonus:
            return &s_bonus_actor;
        case actor_heart:
            return &s_heart_actor;
        case actor_donkey_kong:
            return &s_donkey_kong_actor;
        default:
            return NULL;
    }
}

void actor_animation(actor_t* actor, animations_t animation, uint32_t ticks) {
    if (actor->animation_type == animation)
        return;

    actor->frame = 0;
    actor->animation_type = animation;

    switch (animation) {
        case anim_none:
            actor->animation = NULL;
            break;
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
        case anim_bonus_100:
            actor->animation = &s_bonus_100_anim;
            break;
        case anim_bonus_200:
            actor->animation = &s_bonus_200_anim;
            break;
        case anim_bonus_300:
            actor->animation = &s_bonus_300_anim;
            break;
        case anim_bonus_500:
            actor->animation = &s_bonus_500_anim;
            break;
        case anim_bonus_800:
            actor->animation = &s_bonus_800_anim;
            break;
        case anim_mario_climb:
            actor->animation = &s_mario_climb;
            break;
        case anim_mario_climb_end:
            actor->animation = &s_mario_climb_end;
            break;
        case anim_mario_hammer_walk_left:
            actor->animation = NULL;
            break;
        case anim_mario_hammer_walk_right:
            actor->animation = NULL;
            break;
        case anim_mario_die:
            actor->animation = NULL;
            break;
        case anim_pauline_stand_right:
            actor->animation = &s_pauline_stand_right;
            break;
        case anim_pauline_stand_left:
            actor->animation = &s_pauline_stand_left;
            break;
        case anim_pauline_shuffle_right:
            actor->animation = &s_pauline_shuffle_right;
            break;
        case anim_pauline_shuffle_left:
            actor->animation = &s_pauline_shuffle_left;
            break;
        case anim_barrel_roll_right:
            actor->animation = NULL;
            break;
        case anim_barrel_roll_left:
            actor->animation = NULL;
            break;
        case anim_barrel_roll_down:
            actor->animation = NULL;
            break;
        case anim_barrel_stacked:
            actor->animation = NULL;
            break;
        case anim_donkey_kong_climb_ladder:
            actor->animation = &s_donkey_kong_climb;
            break;
        case anim_donkey_kong_roar:
            actor->animation = NULL;
            break;
        case anim_donkey_kong_throw_barrel:
            actor->animation = NULL;
            break;
        case anim_donkey_kong_title_pose:
            actor->animation = &s_donkey_kong_title_pose;
            break;
        case anim_oil_barrel:
            actor->animation = &s_oil_barrel_anim;
            break;
        case anim_oil_fire:
            actor->animation = &s_oil_fire_anim;
            break;
        default:
            actor->animation = NULL;
            break;
    }

    actor->next_tick = ticks + actor->animation->frames[0].delay;
}
