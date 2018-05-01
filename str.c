#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "str.h"

// N.B. not thread safe
static char buffer[65537];

str_t* str_clone(const char* str) {
    str_t* string = (str_t*) malloc(sizeof(str_t));
    if (string == NULL) {
        abort();
    }
    string->data = (uint8_t*) strdup(str);
    string->len = (uint16_t) strlen(str);
    return string;
}

str_t* str_new(uint16_t len) {
    str_t* string = (str_t*) malloc(sizeof(str_t));
    if (string == NULL) {
        abort();
    }
    string->data = (uint8_t*) malloc(len);
    string->len = len;
    return string;
}

void str_free(str_t* str) {
    assert(str != NULL);
    assert(str->data != NULL);
    free(str);
}

// "hello" == 5
uint16_t str_len(str_t* str) {
    assert(str != NULL);
    return str->len;
}

// concat("hello ", "world") == "hello world"
str_t* str_concat(const str_t* lhs, const str_t* rhs) {
    assert(lhs != NULL);
    assert(rhs != NULL);

    str_t* str = (str_t*) malloc(sizeof(str_t));
    str->data = (uint8_t*) malloc(lhs->len + rhs->len);
    str->len = lhs->len + rhs->len;
    memcpy(str->data, lhs->data, lhs->len);
    memcpy(str->data + lhs->len, rhs->data, rhs->len);
    return str;
}

// left("hello world", 5) == "hello"
str_t* str_left(const str_t* lhs, uint16_t len) {
    assert(lhs != NULL);
    assert(len > 0 && len <= lhs->len);

    str_t* str = (str_t*) malloc(sizeof(str_t));
    str->data = (uint8_t*) malloc(len);
    str->len = len;
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
    memcpy(
        str->data,
        (lhs->data + (lhs->len - 1)) - len,
        len);
    return str;
}

void str_print(str_t* str, FILE* file) {
    assert(str != NULL);
    assert(file != NULL);

    memcpy(&buffer, str->data, str->len);
    buffer[str->len] = '\0';

    fprintf(file, "%s", buffer);
}
