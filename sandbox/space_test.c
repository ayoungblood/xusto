// space-test.c
// Compile and run with
//    gcc -Wall -Wextra -o st ../src/space.c ../src/space_hashtable.c ../src/types.c space_test.c && ./st && rm st

#include "../src/space.h"

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    space_t *space = NULL;
    space = space_create(vector3(256,80,4),(1<<17));

    space_destroy(space);
    return 0;
}
