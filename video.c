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
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_surface.h>
#include "log.h"
#include "tile.h"
#include "video.h"
#include "sprite.h"
#include "window.h"
#include "palette.h"
#include "tile_map.h"

static rect_t s_clip_rect;

static SDL_Surface* s_bg_surface;

static SDL_Surface* s_fg_surface;

static spr_control_block_t s_spr_control[SPRITE_MAX];

static bg_control_block_t s_bg_control[TILE_MAP_SIZE];

void video_bg_str(
        const char* str,
        uint8_t y,
        uint8_t x,
        uint8_t palette) {
    assert(str != NULL);

    size_t length = strlen(str);
    for (uint8_t i = 0; i < length; i++) {
        bg_control_block_t* block = video_tile(y, x + i);
        uint8_t c = (uint8_t) str[i];
        if (c == 0x20) {
            block->tile = 0x0a;
            block->palette = 0x0f;
        } else if (c == '=') {
            block->tile = 52;
            block->palette = palette;
        } else if (c > 128) {
            block->tile = c;
            block->palette = palette;
        } else {
            block->tile = (uint16_t) (c - 48);
            block->palette = palette;
        }
        block->flags |= f_bg_changed;
    }
}

void video_bg_blink(
    uint8_t y,
    uint8_t x,
    uint8_t h,
    uint8_t w,
    uint32_t duration) {
    uint8_t ty = y;
    uint8_t tx = x;
    uint32_t ticks = SDL_GetTicks();
    for (uint8_t i = 0; i < h; i++) {
        for (uint8_t j = 0; j < w; j++) {
            bg_control_block_t* block = video_tile(ty, tx);
            block->blink_visible = false;
            block->blink_duration = duration;
            block->blink_timeout = ticks + duration;
            if (tx < TILE_MAP_WIDTH)
                tx++;
        }
        tx = x;
        if (ty < TILE_MAP_HEIGHT)
            ty++;
    }
}

void video_reset_bg(void) {
    for (uint32_t i = 0; i < TILE_MAP_SIZE; i++) {
        s_bg_control[i].tile = 0;
        s_bg_control[i].palette = 0;
        s_bg_control[i].blink_timeout = 0;
        s_bg_control[i].blink_duration = 0;
        s_bg_control[i].blink_visible = false;
        s_bg_control[i].flags = f_bg_enabled | f_bg_changed;
    }
}

static void video_bg_update(void) {
    SDL_LockSurface(s_bg_surface);

    uint32_t tx = 0;
    uint32_t ty = 0;
    uint32_t ticks = SDL_GetTicks();

    for (uint32_t i = 0; i < TILE_MAP_SIZE; i++) {
        bg_control_block_t* block = &s_bg_control[i];

        uint16_t tile_index = block->tile;
        uint8_t palette_index = block->palette;

        if (block->blink_duration > 0) {
            if (ticks > block->blink_timeout) {
                block->flags |= f_bg_changed;
                block->blink_timeout = ticks + block->blink_duration;
                block->blink_visible = !block->blink_visible;
                if (!block->blink_visible) {
                    tile_index = 0x0a;
                    palette_index = 0x0f;
                }
            }
        }

        if ((block->flags & f_bg_changed) == 0)
            goto next_tile;

        if ((block->flags & f_bg_enabled) == 0)
            goto next_tile;

        const palette_t* pal = palette(palette_index);
        if (pal == NULL)
            goto next_tile;

        const tile_bitmap_t* bitmap = tile_bitmap(tile_index);
        if (bitmap == NULL)
            goto next_tile;

        bool selected = (block->flags & f_bg_select) != 0;
        bool vertical_flip = (block->flags & f_bg_vflip) != 0;
        bool horizontal_flip = (block->flags & f_bg_hflip) != 0;

        uint8_t sy = (uint8_t) (vertical_flip ? TILE_HEIGHT - 1 : 0);
        int8_t syd = (int8_t) (vertical_flip ? -1 : 1);
        int8_t sxd = (int8_t) (horizontal_flip ? -1 : 1);

        for (uint32_t y = 0; y < TILE_HEIGHT; y++) {
            uint8_t* p = s_bg_surface->pixels + ((ty + y) * s_bg_surface->pitch + (tx * 4));
            uint8_t sx = (uint8_t) (horizontal_flip ? TILE_WIDTH - 1 : 0);
            for (uint32_t x = 0; x < TILE_WIDTH; x++) {
                const uint32_t pixel_offset = (const uint32_t) (sy * TILE_WIDTH + sx);
                if (selected && ((x == 0 || x == TILE_WIDTH - 1) || (y == 0 || y == TILE_HEIGHT - 1))) {
                    *p++ = 0xff;
                    *p++ = 0xff;
                    *p++ = 0xff;
                    *p++ = 0xff;
                } else {
                    const palette_entry_t* pal_entry = &pal->entries[bitmap->data[pixel_offset]];
                    *p++ = pal_entry->red;
                    *p++ = pal_entry->green;
                    *p++ = pal_entry->blue;
                    *p++ = 0xff;
                }
                sx += sxd;
            }
            sy += syd;
        }

        block->flags &= ~f_bg_changed;

    next_tile:
        tx += TILE_WIDTH;
        if (tx == SCREEN_WIDTH) {
            tx = 0;
            ty += TILE_HEIGHT;
        }
    }

    SDL_UnlockSurface(s_bg_surface);

    SDL_BlitSurface(s_bg_surface, NULL, s_fg_surface, NULL);
}

static void video_fg_update(void) {
    SDL_LockSurface(s_fg_surface);
    for (uint32_t i = 0; i < SPRITE_MAX; i++) {
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

        uint8_t sy = (uint8_t) (vertical_flip ? SPRITE_HEIGHT - 1 : 0);
        int8_t syd = (int8_t) (vertical_flip ? -1 : 1);
        int8_t sxd = (int8_t) (horizontal_flip ? -1 : 1);

        for (uint32_t y = 0; y < SPRITE_HEIGHT; y++) {
            uint32_t ty = block->y + y;
            if (ty > s_clip_rect.top
            &&  ty < s_clip_rect.top + s_clip_rect.height) {
                uint8_t* p = s_fg_surface->pixels + (ty * s_fg_surface->pitch + (block->x * 4));
                uint8_t sx = (uint8_t) (horizontal_flip ? SPRITE_WIDTH - 1 : 0);
                for (uint32_t x = 0; x < SPRITE_WIDTH; x++) {
                    uint32_t tx = block->x + x;
                    if (tx < s_clip_rect.left || tx > s_clip_rect.left + s_clip_rect.width) {
                        p += 4;
                    } else {
                        const uint32_t pixel_offset = (const uint32_t) (sy * SPRITE_WIDTH + sx);
                        const palette_entry_t* pal_entry = &pal->entries[bitmap->data[pixel_offset]];
                        if (pal_entry->alpha == 0x00)
                            p += 4;
                        else {
                            *p++ = pal_entry->red;
                            *p++ = pal_entry->green;
                            *p++ = pal_entry->blue;
                            *p++ = pal_entry->alpha;
                        }
                    }
                    sx += sxd;
                }
            }
            sy += syd;
        }

        block->flags &= ~f_spr_changed;
    }
    SDL_UnlockSurface(s_fg_surface);
}

void video_init(void) {
//    rect_t temp = {.left = 64, .top = 32, .width = 128, .height = 224};
//    video_clip_rect(temp);

    video_clip_rect_clear();

    log_message(category_video, "allocate RGBA8888 bg surface.");
    s_bg_surface = SDL_CreateRGBSurfaceWithFormat(
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        32,
        SDL_PIXELFORMAT_RGBA8888);
    log_message(category_video, "set s_bg_surface blend mode: none.");
    SDL_SetSurfaceBlendMode(s_bg_surface, SDL_BLENDMODE_NONE);

    log_message(category_video, "allocate RGBA8888 fg surface.");
    s_fg_surface = SDL_CreateRGBSurfaceWithFormat(
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        32,
        SDL_PIXELFORMAT_RGBA8888);
    log_message(category_video, "set s_fg_surface blend mode: none.");
    SDL_SetSurfaceBlendMode(s_fg_surface, SDL_BLENDMODE_NONE);
    log_message(category_video, "set s_bg_surface RLE enabled.");
    SDL_SetSurfaceRLE(s_fg_surface, SDL_TRUE);
}

void video_update(void) {
    video_bg_update();
    video_fg_update();
}

void video_shutdown(void) {
    log_message(category_video, "free bg surface.");
    SDL_FreeSurface(s_bg_surface);
    log_message(category_video, "free fg surface.");
    SDL_FreeSurface(s_fg_surface);
}

void video_reset_sprites(void) {
    for (uint32_t i = 0; i < SPRITE_MAX; i++) {
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

void video_clip_rect_clear(void) {
    s_clip_rect.top = 8;
    s_clip_rect.left = 0;
    s_clip_rect.width = SCREEN_WIDTH;
    s_clip_rect.height = (int16_t) (SCREEN_HEIGHT - s_clip_rect.top);
}

void video_clip_rect(rect_t rect) {
    s_clip_rect.top = rect.top;
    s_clip_rect.left = rect.left;
    s_clip_rect.width = rect.width;
    s_clip_rect.height = rect.height;
}

void video_set_bg(const tile_map_t* map) {
    assert(map != NULL);

    for (uint32_t i = 0; i < TILE_MAP_SIZE; i++) {
        s_bg_control[i].blink_timeout = 0;
        s_bg_control[i].blink_duration = 0;
        s_bg_control[i].blink_visible = false;
        s_bg_control[i].tile = map->data[i].tile;
        s_bg_control[i].palette = map->data[i].palette;
        s_bg_control[i].flags = map->data[i].flags | f_bg_enabled | f_bg_changed;
    }
}

spr_control_block_t* video_sprite(uint8_t number) {
    return &s_spr_control[number];
}

bg_control_block_t* video_tile(uint8_t y, uint8_t x) {
    uint32_t index = (uint32_t) (y * TILE_MAP_WIDTH + x);
    return &s_bg_control[index];
}
