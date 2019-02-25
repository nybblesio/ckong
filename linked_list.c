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
#include "linked_list.h"

ll_node_t* ll_new_node() {
    ll_node_t* node = malloc(sizeof(ll_node_t));
    if (node == NULL)
        abort();
    node->data = NULL;
    node->prev = NULL;
    node->next = NULL;
    node->key = 0;
    return node;
}

void ll_free(ll_node_t* node) {
    assert(node != NULL);

    ll_node_t* current = node;
    while (1) {
        current->prev = NULL;
        ll_node_t* next = current->next;
        free(current);
        if (next == NULL)
            break;
        current = next;
    }
}

uint32_t ll_size(ll_node_t* node) {
    uint32_t size = 0;
    ll_node_t* current = node;
    while (1) {
        ++size;
        if (current->next == NULL)
            break;
        current = current->next;
    }
    return size;
}

bool ll_is_head(const ll_node_t* node) {
    return node != NULL && node->prev == NULL;
}

bool ll_is_tail(const ll_node_t* node) {
    return node != NULL && node->next == NULL;
}

void ll_insert_after(ll_node_t* node, ll_node_t* new_node) {
    assert(node != NULL);
    assert(new_node != NULL);

    if (node->next != NULL) {
        ll_node_t* current_next = node->next;
        current_next->prev = new_node;
        new_node->next = current_next;
    }
    node->next = new_node;
    new_node->prev = node;
}

void ll_insert_before(ll_node_t* node, ll_node_t* new_node) {
    assert(node != NULL);
    assert(new_node != NULL);

    if (node->prev != NULL) {
        ll_node_t* current_prev = node->prev;
        current_prev->next = new_node;
        new_node->prev = current_prev;
    }
    node->prev = new_node;
    new_node->next = node;
}
