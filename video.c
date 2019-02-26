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
#include <SDL_timer.h>
#include <SDL_surface.h>
#include <SDL_FontCache.h>
#include "log.h"
#include "tile.h"
#include "video.h"
#include "sprite.h"
#include "window.h"
#include "palette.h"
#include "tile_map.h"

static rect_t s_clip_rect;

static FC_Font* s_font;

static SDL_Surface* s_bg_surface;

static SDL_Surface* s_fg_surface;

static bg_blinker_t s_blinkers[BLINKERS_MAX];
static uint32_t s_current_blinker = 0;

static vid_pre_command_t s_pre_commands[PRE_COMMANDS_MAX];
static vid_post_command_t s_post_commands[POST_COMMANDS_MAX];
static uint32_t s_current_pre_command = 0;
static uint32_t s_current_post_command = 0;

static spr_control_block_t s_spr_control[SPRITE_MAX];

static bg_control_block_t s_bg_control[TILE_MAP_SIZE];

void video_bg_str(
        uint8_t y,
        uint8_t x,
        uint8_t palette,
        bool enabled,
        const char* fmt,
        ...) {
    assert(fmt != NULL);

    char buffer[33];

    va_list list;
    va_start(list, fmt);
    vsnprintf(buffer, 33, fmt, list);
    va_end(list);

    size_t length = strlen(buffer);
    for (uint8_t i = 0; i < length; i++) {
        bg_control_block_t* block = video_tile(y, x + i);
        uint8_t c = (uint8_t) buffer[i];
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
        if (!enabled)
            block->flags &= ~f_bg_enabled;
        block->flags |= f_bg_changed;
    }
}

bg_blinker_t* video_bg_blink(
        uint8_t y,
        uint8_t x,
        uint8_t h,
        uint8_t w,
        uint32_t ticks,
        uint32_t duration,
        bg_blinker_callback callback) {
    assert(s_current_blinker < BLINKERS_MAX);

    bg_blinker_t* blinker = &s_blinkers[s_current_blinker++];

    rect_t bounds = {
        .left = x,
        .top = y,
        .width = w,
        .height = h
    };
    blinker->bounds = bounds;
    blinker->visible = false;
    blinker->duration = duration;
    blinker->callback = callback;
    blinker->timeout = ticks + duration;

    return blinker;
}

void video_bg_reset(void) {
    s_current_blinker = 0;
    for (uint32_t i = 0; i < TILE_MAP_SIZE; i++) {
        s_bg_control[i].tile = 0;
        s_bg_control[i].palette = 0;
        s_bg_control[i].flags = f_bg_enabled | f_bg_changed;
    }
}

static bool video_draw_spr(
        SDL_Surface* surface,
        uint16_t px,
        uint16_t py,
        uint16_t tile_index,
        uint8_t pal_index,
        uint8_t flags) {
    const palette_t* pal = palette(pal_index);
    if (pal == NULL)
        return false;

    const sprite_bitmap_t* bitmap = sprite_bitmap(tile_index);
    if (bitmap == NULL)
        return false;

    const bool horizontal_flip = (flags & f_spr_hflip) == f_spr_hflip;
    const bool vertical_flip = (flags & f_spr_vflip) == f_spr_vflip;

    uint8_t sy = (uint8_t) (vertical_flip ? SPRITE_HEIGHT - 1 : 0);
    int8_t syd = (int8_t) (vertical_flip ? -1 : 1);
    int8_t sxd = (int8_t) (horizontal_flip ? -1 : 1);

    for (uint32_t y = 0; y < SPRITE_HEIGHT; y++) {
        uint32_t ty = py + y;
        if (ty > s_clip_rect.top
        &&  ty < s_clip_rect.top + s_clip_rect.height) {
            uint8_t* p = surface->pixels + (ty * surface->pitch + (px * 4));
            uint8_t sx = (uint8_t) (horizontal_flip ? SPRITE_WIDTH - 1 : 0);
            for (uint32_t x = 0; x < SPRITE_WIDTH; x++) {
                uint32_t tx = px + x;
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

    return true;
}

static bool video_draw_tile(
        SDL_Surface* surface,
        uint16_t tx,
        uint16_t ty,
        uint16_t tile_index,
        uint8_t pal_index,
        uint8_t flags) {
    const palette_t* pal = palette(pal_index);
    if (pal == NULL)
        return false;

    const tile_bitmap_t* bitmap = tile_bitmap(tile_index);
    if (bitmap == NULL)
        return false;

    const bool vertical_flip = (flags & f_bg_vflip) == f_bg_vflip;
    const bool horizontal_flip = (flags & f_bg_hflip) == f_bg_hflip;

    uint8_t sy = (uint8_t) (vertical_flip ? TILE_HEIGHT - 1 : 0);
    int8_t syd = (int8_t) (vertical_flip ? -1 : 1);
    int8_t sxd = (int8_t) (horizontal_flip ? -1 : 1);

    for (uint32_t y = 0; y < TILE_HEIGHT; y++) {
        uint8_t* p = surface->pixels + ((ty + y) * surface->pitch + (tx * 4));
        uint8_t sx = (uint8_t) (horizontal_flip ? TILE_WIDTH - 1 : 0);
        for (uint32_t x = 0; x < TILE_WIDTH; x++) {
            const uint32_t pixel_offset = (const uint32_t) (sy * TILE_WIDTH + sx);
            const palette_entry_t* pal_entry = &pal->entries[bitmap->data[pixel_offset]];
            *p++ = pal_entry->red;
            *p++ = pal_entry->green;
            *p++ = pal_entry->blue;
            *p++ = 0xff;
            sx += sxd;
        }
        sy += syd;
    }

    return true;
}

static void video_bg_update(uint32_t ticks) {
    for (uint32_t i = 0; i < s_current_blinker; i++) {
        bg_blinker_t* blinker = &s_blinkers[i];
        if (blinker->duration > 0) {
            if (ticks > blinker->timeout) {
                bool blinker_result = true;

                if (blinker->callback != NULL) {
                    blinker_result = blinker->callback(blinker, ticks);
                }

                if (blinker_result) {
                    blinker->timeout = ticks + blinker->duration;
                }

                int32_t ty = blinker->bounds.top;
                int32_t tx = blinker->bounds.left;
                for (uint8_t y = 0; y < blinker->bounds.height; y++) {
                    for (uint8_t x = 0; x < blinker->bounds.width; x++) {
                        bg_control_block_t* block = video_tile(
                            (uint8_t) ty,
                            (uint8_t) tx);
                        if (!blinker->visible) {
                            block->flags &= ~f_bg_enabled;
                        } else {
                            block->flags |= f_bg_enabled;
                        }
                        block->flags |= f_bg_changed;

                        if (tx < TILE_MAP_WIDTH)
                            tx++;
                    }
                    tx = blinker->bounds.left;
                    if (ty < TILE_MAP_HEIGHT)
                        ty++;
                }

                blinker->visible = !blinker->visible;
            }
        }
    }

    uint32_t tx = 0;
    uint32_t ty = 0;
    SDL_LockSurface(s_bg_surface);

    for (uint32_t i = 0; i < TILE_MAP_SIZE; i++) {
        bg_control_block_t* block = &s_bg_control[i];

        uint16_t tile_index = block->tile;
        uint8_t palette_index = block->palette;

        if ((block->flags & f_bg_changed) == 0)
            goto next_tile;

        if ((block->flags & f_bg_enabled) == 0) {
            tile_index = 0x0a;
            palette_index = 0x0f;
        }

        if (video_draw_tile(
                s_bg_surface,
                tx,
                ty,
                tile_index,
                palette_index,
                block->flags)) {
            block->flags &= ~f_bg_changed;
        }

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

static void video_fg_update(uint32_t ticks) {
    for (uint32_t i = 0; i < SPRITE_MAX; i++) {
        spr_control_block_t* block = &s_spr_control[i];

        if ((block->flags & f_spr_enabled) == 0)
            continue;

        if (!video_draw_spr(
                s_fg_surface,
                block->x,
                block->y,
                block->tile,
                block->palette,
                block->flags)) {
            continue;
        }

        block->flags &= ~f_spr_changed;
    }
}

static void vline(uint16_t y, uint16_t x, uint16_t h, color_t* color) {
    for (uint32_t h1 = 0; h1 < h; h1++) {
        uint8_t* p = s_fg_surface->pixels + ((y + h1) * s_fg_surface->pitch + (x * 4));
        *p++ = color->r;
        *p++ = color->g;
        *p++ = color->b;
        *p = color->a;
    }
}

static void hline(uint16_t y, uint16_t x, uint16_t w, color_t* color) {
    uint8_t* p = s_fg_surface->pixels + ((y + 0) * s_fg_surface->pitch + (x * 4));
    for (uint32_t w1 = 0; w1 < w; w1++) {
        *p++ = color->r;
        *p++ = color->g;
        *p++ = color->b;
        *p++ = color->a;
    }
}

static void video_pre_commands(uint32_t ticks) {
    for (uint16_t i = 0; i < s_current_pre_command; i++) {
        vid_pre_command_t* cmd = &s_pre_commands[i];
        switch (cmd->type) {
            case vid_pre_spr: {
                const vid_tile_data_t* tile = &cmd->data.tile;
                video_draw_spr(
                    s_fg_surface,
                    tile->x,
                    tile->y,
                    tile->tile,
                    tile->palette,
                    tile->flags);
                break;
            }
            case vid_pre_tile: {
                const vid_tile_data_t* tile = &cmd->data.tile;
                video_draw_tile(
                    s_fg_surface,
                    tile->x,
                    tile->y,
                    tile->tile,
                    tile->palette,
                    tile->flags);
                break;
            }
            case vid_pre_hline: {
                vid_hline_data_t* line = &cmd->data.hline;
                hline(line->y, line->x, line->w, &line->color);
                break;
            }
            case vid_pre_vline: {
                vid_vline_data_t* line = &cmd->data.vline;
                vline(line->y, line->x, line->h, &line->color);
                break;
            }
            case vid_pre_rect: {
                vid_rect_data_t* vid_rect = &cmd->data.rect;
                SDL_Rect rect = {
                    vid_rect->bounds.left,
                    vid_rect->bounds.top,
                    vid_rect->bounds.width,
                    vid_rect->bounds.height
                };
                if (cmd->data.rect.fill) {
                    for (uint32_t l = 0; l < rect.h; l++)
                        hline(rect.y + l, rect.x, rect.w, &vid_rect->color);
                } else {
                    vline(rect.y,          rect.x,          rect.h,     &vid_rect->color);
                    vline(rect.y,          rect.x + rect.w, rect.h,     &vid_rect->color);
                    hline(rect.y,          rect.x,          rect.w,     &vid_rect->color);
                    hline(rect.y + rect.h, rect.x,          rect.w + 1, &vid_rect->color);
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    s_current_pre_command = 0;
}

static void video_post_commands(struct SDL_Renderer* renderer, uint32_t ticks) {
    for (uint16_t i = 0; i < s_current_post_command; i++) {
        vid_post_command_t* cmd = &s_post_commands[i];
        switch (cmd->type) {
            case vid_post_text: {
                vid_text_data_t* text = &cmd->data.text;
                FC_DrawColor(
                    s_font,
                    renderer,
                    text->x,
                    text->y,
                    FC_MakeColor(
                        text->color.r,
                        text->color.g,
                        text->color.b,
                        text->color.a),
                    text->buffer);
                break;
            }
            default: {
                break;
            }
        }
    }

    s_current_post_command = 0;
}

void video_init(struct SDL_Renderer* renderer) {
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

    log_message(category_video, "load font: assets/press-start-2p.ttf");
    s_font = FC_CreateFont();
    FC_LoadFont(
        s_font,
        renderer,
        "../assets/press-start-2p.ttf",
        8,
        FC_MakeColor(0xff, 0xff, 0xff, 0xff),
        TTF_STYLE_NORMAL);
}

void video_update(window_t* window, uint32_t ticks) {
    video_bg_update(ticks);

    SDL_LockSurface(s_fg_surface);
    video_fg_update(ticks);
    video_pre_commands(ticks);
    SDL_UnlockSurface(s_fg_surface);

    SDL_UpdateTexture(
        window->texture,
        NULL,
        s_fg_surface->pixels,
        s_fg_surface->pitch);

    SDL_RenderCopy(
        window->renderer,
        window->texture,
        NULL,
        NULL);

    video_post_commands(window->renderer, ticks);

    SDL_RenderPresent(window->renderer);
}

void video_shutdown(void) {
    log_message(category_video, "free bg surface.");
    SDL_FreeSurface(s_bg_surface);
    log_message(category_video, "free fg surface.");
    SDL_FreeSurface(s_fg_surface);
    log_message(category_video, "free font.");
    FC_FreeFont(s_font);
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

void video_bg_set(const tile_map_t* map) {
    assert(map != NULL);

    for (uint32_t i = 0; i < TILE_MAP_SIZE; i++) {
        s_bg_control[i].tile = map->data[i].tile;
        s_bg_control[i].palette = map->data[i].palette;
        s_bg_control[i].flags = map->data[i].flags | f_bg_enabled | f_bg_changed;
    }
}

void video_bg_fill(uint16_t tile, uint8_t palette) {
    for (uint32_t i = 0; i < TILE_MAP_SIZE; i++) {
        s_bg_control[i].tile = tile;
        s_bg_control[i].palette = palette;
        s_bg_control[i].flags |= f_bg_enabled | f_bg_changed;
    }
}

spr_control_block_t* video_sprite(uint8_t number) {
    return &s_spr_control[number];
}

bg_control_block_t* video_tile(uint8_t y, uint8_t x) {
    uint32_t index = (uint32_t) (y * TILE_MAP_WIDTH + x);
    return &s_bg_control[index];
}

void video_rect(color_t color, rect_t rect) {
    if (s_current_pre_command >= PRE_COMMANDS_MAX - 1)
        return;

    s_pre_commands[s_current_pre_command].type = vid_pre_rect;
    s_pre_commands[s_current_pre_command].data.rect.fill = false;
    s_pre_commands[s_current_pre_command].data.rect.color = color;
    s_pre_commands[s_current_pre_command].data.rect.bounds = rect;
    ++s_current_pre_command;
}

void video_fill_rect(color_t color, rect_t rect) {
    if (s_current_pre_command >= PRE_COMMANDS_MAX - 1)
        return;

    s_pre_commands[s_current_pre_command].type = vid_pre_rect;
    s_pre_commands[s_current_pre_command].data.rect.fill = true;
    s_pre_commands[s_current_pre_command].data.rect.color = color;
    s_pre_commands[s_current_pre_command].data.rect.bounds = rect;
    ++s_current_pre_command;
}

void video_vline(color_t color, uint16_t y, uint16_t x, uint16_t h) {
    if (s_current_pre_command >= PRE_COMMANDS_MAX - 1)
        return;

    vid_vline_data_t line = {
        .color = color,
        .x = x,
        .y = y,
        .h = h
    };
    s_pre_commands[s_current_pre_command].type = vid_pre_vline;
    s_pre_commands[s_current_pre_command].data.vline = line;
    ++s_current_pre_command;
}

void video_hline(color_t color, uint16_t y, uint16_t x, uint16_t w) {
    if (s_current_pre_command >= PRE_COMMANDS_MAX - 1)
        return;

    s_pre_commands[s_current_pre_command].type = vid_pre_hline;

    vid_hline_data_t line = {
        .color = color,
        .x = x,
        .y = y,
        .w = w
    };
    s_pre_commands[s_current_pre_command].data.hline = line;
    ++s_current_pre_command;
}

void video_text(color_t color, uint16_t y, uint16_t x, const char* fmt, ...) {
    if (s_current_post_command >= POST_COMMANDS_MAX - 1)
        return;

    s_post_commands[s_current_post_command].type = vid_post_text;

    vid_text_data_t text = {
        .color = color,
        .x = x,
        .y = y,
    };

    va_list list;
    va_start(list, fmt);
    vsnprintf(text.buffer, 256, fmt, list);
    va_end(list);

    s_post_commands[s_current_post_command].data.text = text;
    ++s_current_post_command;
}

void video_stamp_tile(uint16_t y, uint16_t x, uint16_t tile, uint8_t palette, uint8_t flags) {
    if (s_current_pre_command >= POST_COMMANDS_MAX - 1)
        return;

    s_pre_commands[s_current_pre_command].type = vid_pre_tile;

    vid_tile_data_t tile_data = {
        .x = x,
        .y = y,
        .tile = tile,
        .flags = flags,
        .palette = palette,
    };

    s_pre_commands[s_current_pre_command].data.tile = tile_data;
    ++s_current_pre_command;
}
