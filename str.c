#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "str.h"

// N.B. not thread safe
static char s_buffer[65537];

void str_free(str_t* str) {
    assert(str != NULL);
    assert(str->data != NULL);
    free(str);
}

str_t* str_new(uint16_t len) {
    str_t* str = (str_t*) malloc(sizeof(str_t));
    if (str == NULL)
        abort();
    str->data = (uint8_t*) malloc(len);
    str->len = len;
    str->pos = str->len;
    return str;
}

str_t* str_clone(const char* lhs) {
    str_t* str = (str_t*) malloc(sizeof(str_t));
    if (str == NULL)
        abort();
    str->data = (uint8_t*) strdup(lhs);
    str->len = (uint16_t) strlen(lhs);
    str->pos = str->len;
    return str;
}

// "hello" == 5
uint16_t str_len(const str_t* str) {
    assert(str != NULL);
    return str->len;
}

void str_print(const str_t* str, FILE* file) {
    assert(str != NULL);
    assert(file != NULL);

    memcpy(&s_buffer, str->data, str->len);
    s_buffer[str->len] = '\0';

    fprintf(file, "%s", s_buffer);
}

// left("hello world", 5) == "hello"
str_t* str_left(const str_t* lhs, uint16_t len) {
    assert(lhs != NULL);
    assert(len > 0 && len <= lhs->len);

    str_t* str = (str_t*) malloc(sizeof(str_t));
    str->data = (uint8_t*) malloc(len);
    str->len = len;
    str->pos = len;
    memcpy(str->data, lhs->data, len);
    return str;
}

// right("foo", 2) == "oo"
str_t* str_right(const str_t* lhs, uint16_t len) {
    assert(lhs != NULL);
    assert(len > 0 && len <= lhs->len);

    str_t* str = (str_t*) malloc(sizeof(str_t));
    str->data = (uint8_t*) malloc(len);
    str->len = len;
    str->pos = len;
    memcpy(
        str->data,
        (lhs->data + (lhs->len - 1)) - len,
        len);
    return str;
}

uint16_t str_append(str_t* str, const str_t* lhs) {
    assert(str != NULL);
    assert(lhs != NULL);

    uint16_t new_len = str->pos + lhs->len;
    if (new_len > str->len) {
        str->data = realloc(str->data, new_len);
        str->len = new_len;
    }
    memcpy(str->data + str->pos, lhs->data, lhs->len);
    return str->len;
}

uint16_t str_appendz(str_t* str, const char* lhs) {
    assert(str != NULL);
    assert(lhs != NULL);

    uint16_t lhs_len = (uint16_t) strlen(lhs);
    uint16_t new_len = str->pos + lhs_len;
    if (new_len > str->len) {
        str->data = realloc(str->data, new_len);
        str->len = new_len;
    }
    memcpy(str->data + str->pos, lhs, lhs_len);
    return str->len;
}

// concat("hello ", "world") == "hello world"
str_t* str_concat(const str_t* lhs, const str_t* rhs) {
    assert(lhs != NULL);
    assert(rhs != NULL);

    str_t* str = (str_t*) malloc(sizeof(str_t));
    str->data = (uint8_t*) malloc(lhs->len + rhs->len);
    str->len = lhs->len + rhs->len;
    str->pos = str->len;
    memcpy(str->data, lhs->data, lhs->len);
    memcpy(str->data + lhs->len, rhs->data, rhs->len);
    return str;
}
