#pragma once

#include <stdio.h>
#include <stdint.h>

typedef struct str {
    uint16_t len;
    uint8_t* data;
} str_t;

void str_free(str_t* str);

str_t* str_new(uint16_t len);

uint16_t str_len(str_t* str);

str_t* str_clone(const char* str);

void str_print(str_t* str, FILE* file);

str_t* str_left(const str_t* lhs, uint16_t len);

str_t* str_right(const str_t* lhs, uint16_t len);

str_t* str_concat(const str_t* lhs, const str_t* rhs);
