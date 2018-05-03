#pragma once

#include <stdbool.h>
#include "str.h"
#include "linked_list.h"
#include "window.h"

typedef struct game_context {
    bool valid;
    window_t window;
    linked_list_node_t* messages;
} game_context_t;

game_context_t* game_context_new();

bool game_run(game_context_t* context);

bool game_init(game_context_t* context);

void game_shutdown(game_context_t* context);
