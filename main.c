#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "str.h"

void test_str_module() {
    str_t* hello_world = str_clone("HELLO WORLD!");
    str_print(hello_world, stdout);

    fprintf(stdout, "\nlen = %d", str_len(hello_world));

    str_printc('\n', stdout);

    str_t* hello = str_left(hello_world, 5);
    str_print(hello, stdout);

    str_printc('\n', stdout);

    str_t* world_bang = str_right(hello_world, 5);
    str_print(world_bang, stdout);

    str_printc('\n', stdout);

    str_t* combined_again = str_concat(hello, world_bang);
    str_print(combined_again, stdout);

    str_free(hello);
    str_free(combined_again);
    str_free(world_bang);
    str_free(hello_world);
}

int main(int argc, char** argv) {
    //test_str_module();

    return 0;
}