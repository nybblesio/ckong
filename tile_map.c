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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "log.h"
#include "tile_map.h"

static tile_map_file_t s_tile_maps;

void tile_map_load(void) {
    if (access("ckong.dat", F_OK) == -1) {
        log_warn(category_app, "ckong.dat file is missing; not loading.");
        return;
    }
    FILE* file = fopen("ckong.dat", "rb");
    fread(&s_tile_maps, sizeof(tile_map_file_t), 1, file);
    fclose(file);
}

void tile_map_save(void) {
    FILE* file = fopen("ckong.dat", "wb");
    fwrite(&s_tile_maps, sizeof(tile_map_file_t), 1, file);
    fclose(file);
}

void tile_map_init(void) {
    strncpy(s_tile_maps.header, "CKONG11*", 8);
    for (uint8_t i = 0; i < TILE_MAP_MAX; i++) {
        for (uint32_t j = 0; j < TILE_MAP_SIZE; j++) {
            s_tile_maps.maps[i].data[j].tile = 0;
            s_tile_maps.maps[i].data[j].flags = 0;
            s_tile_maps.maps[i].data[j].palette = 0;
        }
    }
}

const tile_map_t* tile_map(uint8_t index) {
    return &s_tile_maps.maps[index];
}