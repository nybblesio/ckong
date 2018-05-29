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

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "str.h"

void str_free(str_t* str) {
    assert(str != NULL);

    if (str->data != NULL)
        free(str->data);

    free(str);
}

str_t* str_new(uint16_t len) {
    str_t* str = (str_t*) malloc(sizeof(str_t));
    if (str == NULL)
        abort();
    str->data = (uint8_t*) malloc(len);
    str->len = len;
    str->pos = 0;
    return str;
}

str_t* str_clone(const char* lhs) {
    str_t* str = str_new((uint16_t) strlen(lhs));
    memcpy(str->data, lhs, str->len);
    str->pos = str->len;
    return str;
}

uint16_t str_pos(const str_t* str) {
    assert(str != NULL);
    return str->pos;
}

// "hello" == 5
uint16_t str_size(const str_t* str) {
    assert(str != NULL);
    return str->len;
}

void str_printc(char c, FILE* file) {
    assert(file != NULL);
    fwrite(&c, sizeof(char), 1, file);
}

void str_print(const str_t* str, FILE* file) {
    assert(str != NULL);
    assert(file != NULL);
    fwrite(str->data, sizeof(uint8_t), str->len, file);
}

// left("hello world", 5) == "hello"
str_t* str_left(const str_t* lhs, uint16_t len) {
    assert(lhs != NULL);
    assert(len > 0 && len <= lhs->len);

    str_t* str = str_new(len);
    memcpy(str->data, lhs->data, len);
    str->pos = len;
    return str;
}

// right("foo", 2) == "oo"
str_t* str_right(const str_t* lhs, uint16_t len) {
    assert(lhs != NULL);
    assert(len > 0 && len <= lhs->len);

    str_t* str = str_new(len);
    memcpy(
        str->data,
        (lhs->data + lhs->len) - len,
        len);
    str->pos = len;
    return str;
}

uint16_t str_append(str_t* str, const str_t* lhs) {
    assert(str != NULL);
    assert(lhs != NULL);

    uint16_t new_len = str->pos + lhs->len;
    if (new_len > str->len)
        str->data = realloc(str->data, new_len);
    str->len = new_len;
    memcpy(str->data + str->pos, lhs->data, lhs->len);
    return str->len;
}

uint16_t str_appendz(str_t* str, const char* lhs) {
    assert(str != NULL);
    assert(lhs != NULL);

    str_t temp;
    temp.data = (uint8_t*) lhs;
    temp.len = (uint16_t) strlen(lhs);
    temp.pos = temp.len;
    return str_append(str, &temp);
}

// concat("hello ", "world") == "hello world"
str_t* str_concat(const str_t* lhs, const str_t* rhs) {
    assert(lhs != NULL);
    assert(rhs != NULL);

    str_t* str = str_new(lhs->len + rhs->len);
    memcpy(str->data, lhs->data, lhs->len);
    memcpy(str->data + lhs->len, rhs->data, rhs->len);
    str->pos = str->len;
    return str;
}
