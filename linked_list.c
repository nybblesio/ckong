#include <assert.h>
#include "linked_list.h"

void linked_list_insert_after(
        linked_list_node_t* node,
        linked_list_node_t* new_node) {
    assert(node != NULL);
    assert(new_node != NULL);

    if (node->next != NULL) {
        linked_list_node_t* current_next = node->next;
        current_next->prev = new_node;
        new_node->next = current_next;
    }
    node->next = new_node;
    new_node->prev = node;
}

void linked_list_insert_before(
        linked_list_node_t* node,
        linked_list_node_t* new_node) {
    assert(node != NULL);
    assert(new_node != NULL);

    if (node->prev != NULL) {
        linked_list_node_t* current_prev = node->prev;
        current_prev->next = new_node;
        new_node->prev = current_prev;
    }
    node->prev = new_node;
    new_node->next = node;
}

linked_list_node_t* linked_list_new_node() {
    linked_list_node_t* node = malloc(sizeof(linked_list_node_t));
    if (node == NULL)
        abort();
    node->data = NULL;
    node->prev = NULL;
    node->next = NULL;
    node->key = 0;
    return node;
}

void linked_list_free(linked_list_node_t* node) {
    assert(node != NULL);

    linked_list_node_t* current = node;
    while (1) {
        current->prev = NULL;
        linked_list_node_t* next = current->next;
        free(current);
        if (next == NULL)
            break;
        current = next;
    }
}

uint32_t linked_list_size(linked_list_node_t* node) {
    uint32_t size = 0;
    linked_list_node_t* current = node;
    while (1) {
        ++size;
        if (current->next == NULL)
            break;
        current = current->next;
    }
    return size;
}

bool linked_list_is_head(const linked_list_node_t* node) {
    return node != NULL && node->prev == NULL;
}

bool linked_list_is_tail(const linked_list_node_t* node) {
    return node != NULL && node->next == NULL;
}