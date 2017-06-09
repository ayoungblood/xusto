// space_hashtable_test.c
// Compile and run with
//    gcc -Wall -Wextra -o shtt ../src/types.c ../src/space_hashtable.c space_hashtable_test.c && ./shtt

#include "../src/space_hashtable.h"

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    cell_t cell, val;
    vector3_t vec;
    space_hashtable_t *sht = NULL;
    sht = space_hashtable_create(2);

    vec = vector3(0,0,0);
    cell = space_hashtable_get(sht, vec);
    printf("%lld, %f\n", cell.i, cell.f);

    vec = vector3(0,0,0);
    val.i = 42;
    space_hashtable_set(sht, vec, val);
    cell = space_hashtable_get(sht, vec);
    printf("%lld, %f\n", cell.i, cell.f);

    vec = vector3(1,0,0);
    val.i = 100;
    space_hashtable_set(sht, vec, val);
    cell = space_hashtable_get(sht, vec);
    printf("%lld, %f\n", cell.i, cell.f);

    vec = vector3(2,0,0);
    val.i = 200;
    space_hashtable_set(sht, vec, val);
    cell = space_hashtable_get(sht, vec);
    printf("%lld, %f\n", cell.i, cell.f);

    vec = vector3(2,4,0);
    val.i = 240;
    space_hashtable_set(sht, vec, val);
    cell = space_hashtable_get(sht, vec);
    printf("%lld, %f\n", cell.i, cell.f);

    vec = vector3(2,4,0);
    val.i = 1240;
    space_hashtable_set(sht, vec, val);
    cell = space_hashtable_get(sht, vec);
    printf("%lld, %f\n", cell.i, cell.f);

    return 0;
}
