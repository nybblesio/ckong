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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "log.h"
#include "str.h"
#include "game.h"

void log_messages(linked_list_node_t* node) {
    linked_list_node_t* current_node = node;
    while (current_node != NULL) {
        if (current_node->data != NULL) {
            str_print((const str_t*) current_node->data, stderr);
            str_printc('\n', stderr);
        }
        current_node = current_node->next;
    }
}

int main(int argc, char** argv) {
    int rc = 0;

    log_init();
    log_message(category_app, "C Kong begin.");

    game_context_t* context = game_context_new();

    if (!game_init(context)) {
        log_messages(context->messages);
        log_messages(context->window.messages);
        rc = 1;
    } else {
        if (!game_run(context)) {
            log_messages(context->messages);
            log_messages(context->window.messages);
            rc = 1;
        }

        game_shutdown(context);
    }

    free(context);
    return rc;
}