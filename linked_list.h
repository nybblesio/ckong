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

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct ll_node ll_node_t;

typedef struct ll_node {
    uint32_t key;
    void* data;
    ll_node_t* prev;
    ll_node_t* next;
} ll_node_t;

ll_node_t* ll_new_node();

void ll_free(ll_node_t* node);

uint32_t ll_size(ll_node_t* node);

bool ll_is_head(const ll_node_t* node);

bool ll_is_tail(const ll_node_t* node);

void ll_insert_after(ll_node_t* node, ll_node_t* new_node);

void ll_insert_before(ll_node_t* node, ll_node_t* new_node);
