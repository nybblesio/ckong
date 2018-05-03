#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct linked_list_node {
    uint32_t key;
    void* data;
    struct linked_list_node* prev;
    struct linked_list_node* next;
} linked_list_node_t;

void linked_list_insert_after(
    linked_list_node_t* node,
    linked_list_node_t* new_node);

void linked_list_insert_before(
    linked_list_node_t* node,
    linked_list_node_t* new_node);

linked_list_node_t* linked_list_new_node();

void linked_list_free(linked_list_node_t* node);

uint32_t linked_list_size(linked_list_node_t* node);

bool linked_list_is_head(const linked_list_node_t* node);

bool linked_list_is_tail(const linked_list_node_t* node);
