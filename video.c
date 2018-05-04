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

#include <assert.h>
#include <SDL2/SDL_surface.h>
#include "tile.h"
#include "video.h"
#include "sprite.h"
#include "window.h"
#include "palette.h"
#include "tile_map.h"

static SDL_Surface* s_bg_surface;

static SDL_Surface* s_fg_surface;

static spr_control_block_t s_spr_control[sprite_max];

static bg_control_block_t s_bg_control[tile_map_width * tile_map_height];

static void video_bg_update(void) {
    SDL_LockSurface(s_bg_surface);
    uint32_t tx = 0;
    uint32_t ty = 0;
    const uint32_t tile_map_size = tile_map_height * tile_map_width;
    for (uint32_t i = 0; i < tile_map_size; i++) {
        bg_control_block_t* block = &s_bg_control[i];
        if ((block->flags & f_bg_changed) == 0
        ||  (block->flags & f_bg_enabled) == 0)
            continue;

        const palette_t* pal = palette(block->palette);
        if (pal == NULL)
            continue;

        const tile_bitmap_t* bitmap = tile_bitmap(block->tile);
        if (bitmap == NULL)
            continue;

        bool horizontal_flip = (block->flags & f_bg_hflip) != 0;
        bool vertical_flip = (block->flags & f_bg_vflip) != 0;

        uint8_t sy = (uint8_t) (vertical_flip ? tile_height - 1 : 0);
        int8_t syd = (int8_t) (vertical_flip ? -1 : 1);
        int8_t sxd = (int8_t) (horizontal_flip ? -1 : 1);

        for (uint32_t y = 0; y < tile_height; y++) {
            uint8_t* p = s_bg_surface->pixels + ((ty + y) * s_bg_surface->pitch + (tx * 4));
            uint8_t sx = (uint8_t) (horizontal_flip ? tile_width - 1 : 0);
            for (uint32_t x = 0; x < tile_width; x++) {
                const uint32_t pixel_offset = sy * tile_width + sx;
                const palette_entry_t* pal_entry = &pal->entries[bitmap->data[pixel_offset]];
                *p++ = pal_entry->red;
                *p++ = pal_entry->green;
                *p++ = pal_entry->blue;
                *p++ = 0xff;
                sx += sxd;
            }
            sy += syd;
        }

        block->flags &= ~f_bg_changed;

        tx += tile_width;
        if (tx == screen_width) {
            tx = 0;
            ty += tile_height;
        }
    }
    SDL_UnlockSurface(s_bg_surface);
    SDL_BlitSurface(s_bg_surface, NULL, s_fg_surface, NULL);
}

static void video_fg_update(void) {
    SDL_LockSurface(s_fg_surface);
    for (uint32_t i = 0; i < sprite_max; i++) {
        spr_control_block_t* block = &s_spr_control[i];

        if ((block->flags & f_spr_enabled) == 0)
            continue;

        const palette_t* pal = palette(block->palette);
        if (pal == NULL)
            continue;

        const sprite_bitmap_t* bitmap = sprite_bitmap(block->tile);
        if (bitmap == NULL)
            continue;

        bool horizontal_flip = (block->flags & f_spr_hflip) != 0;
        bool vertical_flip = (block->flags & f_spr_vflip) != 0;

        uint8_t sy = (uint8_t) (vertical_flip ? sprite_height - 1 : 0);
        int8_t syd = (int8_t) (vertical_flip ? -1 : 1);
        int8_t sxd = (int8_t) (horizontal_flip ? -1 : 1);

        for (uint32_t y = 0; y < sprite_height; y++) {
            uint8_t* p = s_fg_surface->pixels +
                ((block->y + y) * s_fg_surface->pitch +
                 (block->x * 4));
            uint8_t sx = (uint8_t) (horizontal_flip ? sprite_width - 1 : 0);
            for (uint32_t x = 0; x < sprite_width; x++) {
                const uint32_t pixel_offset = sy * sprite_width + sx;
                const palette_entry_t* pal_entry = &pal->entries[bitmap->data[pixel_offset]];
                if (pal_entry->alpha == 0x00)
                    p += 4;
                else {
                    *p++ = pal_entry->red;
                    *p++ = pal_entry->green;
                    *p++ = pal_entry->blue;
                    *p++ = pal_entry->alpha;
                }
                sx += sxd;
            }
            sy += syd;
        }

        block->flags &= ~f_spr_changed;
    }
    SDL_UnlockSurface(s_fg_surface);
}

void video_init(void) {
    s_bg_surface = SDL_CreateRGBSurfaceWithFormat(
        0,
        screen_width,
        screen_height,
        32,
        SDL_PIXELFORMAT_RGBA8888);
    SDL_SetSurfaceBlendMode(s_bg_surface, SDL_BLENDMODE_NONE);

    s_fg_surface = SDL_CreateRGBSurfaceWithFormat(
        0,
        screen_width,
        screen_height,
        32,
        SDL_PIXELFORMAT_RGBA8888);
    SDL_SetSurfaceBlendMode(s_fg_surface, SDL_BLENDMODE_NONE);
    SDL_SetSurfaceRLE(s_fg_surface, SDL_TRUE);
}

void video_update(void) {
    video_bg_update();
    video_fg_update();
}

void video_shutdown(void) {
    SDL_FreeSurface(s_bg_surface);
    SDL_FreeSurface(s_fg_surface);
}

void video_reset_sprites(void) {
    for (uint32_t i = 0; i < sprite_max; i++) {
        s_spr_control[i].x = 0;
        s_spr_control[i].y = 0;
        s_spr_control[i].tile = 0;
        s_spr_control[i].palette = 0;
        s_spr_control[i].flags = f_spr_none;
    }
}

SDL_Surface* video_surface(void) {
    return s_fg_surface;
}

void video_set_bg(const tile_map_t* map) {
    assert(map != NULL);

    for (uint32_t i = 0; i < tile_map_height * tile_map_width; i++) {
        s_bg_control[i].tile = map->data[i].tile;
        s_bg_control[i].palette = map->data[i].palette;
        s_bg_control[i].flags = map->data[i].flags | f_bg_enabled | f_bg_changed;
    }
}

spr_control_block_t* video_sprite(uint8_t number) {
    return &s_spr_control[number];
}

bg_control_block_t* video_tile(uint8_t y, uint8_t x) {
    return &s_bg_control[y * tile_map_width + x];
}
