/* src/vector3-test.c
 * Very simple unit tests for vector3_t from src/types.h
 */

#include <stdlib.h>
#include <time.h>

#include "minunit.h"
#include "../src/types.h"
#include "../src/space_hashtable.h"

int tests_run = 0;

static const char *test_space_hashtable_create_destroy() {
    space_hashtable_t *hash = NULL;

    hash = space_hashtable_create(4);
    mu_assert(_FL "incorrect hashtable size", hash->size == 4);
    space_hashtable_destroy(hash);

    hash = space_hashtable_create(1<<8);
    mu_assert(_FL "incorrect hashtable size", hash->size == (1<<8));
    space_hashtable_destroy(hash);

    return 0;
}

static const char *test_space_hashtable_set_get_small() {
    cell_t cell0, cell1, cell2;
    vector3_t vec0, vec1, vec2;
    space_hashtable_t *hash = NULL;

    hash = space_hashtable_create(4);

    vec0 = vector3(0,0,0);
    cell0.i = 1000;
    space_hashtable_set(hash, vec0, cell0);
    vec0 = vector3(1,0,0);
    cell0.i = 1100;
    space_hashtable_set(hash, vec0, cell0);
    vec0 = vector3(2,0,0);
    cell0.i = 1200;
    space_hashtable_set(hash, vec0, cell0);
    vec0 = vector3(3,0,0);
    cell0.i = 1300;
    space_hashtable_set(hash, vec0, cell0);
    vec0 = vector3(3,3,3);
    cell0.i = 1333;
    space_hashtable_set(hash, vec0, cell0);

    vec1 = vector3(0,0,0);
    cell1 = space_hashtable_get(hash, vec1);
    vec2 = vector3(0,0,0);
    cell2 = space_hashtable_get(hash, vec2);
    mu_assert(_FL "cell integer value not equal", cell1.i == cell2.i);

    vec1 = vector3(0,0,0);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 1000 == cell1.i);
    vec1 = vector3(1,0,0);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 1100 == cell1.i);
    vec1 = vector3(2,0,0);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 1200 == cell1.i);
    vec1 = vector3(3,0,0);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 1300 == cell1.i);
    vec1 = vector3(3,3,3);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 1333 == cell1.i);

    // Check that unset values return 0
    vec1 = vector3(200,200,200);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 0 == cell1.i);
    vec1 = vector3(11,22,33);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 0 == cell1.i);
    vec1 = vector3(1717,1616,1515);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 0 == cell1.i);

    // Overwrite some things
    vec0 = vector3(0,0,0);
    cell0.i = 2000;
    space_hashtable_set(hash, vec0, cell0);
    vec0 = vector3(1,0,0);
    cell0.i = 2100;
    space_hashtable_set(hash, vec0, cell0);
    vec0 = vector3(2,0,0);
    cell0.i = 2200;
    space_hashtable_set(hash, vec0, cell0);
    vec0 = vector3(3,0,0);
    cell0.i = 2300;
    space_hashtable_set(hash, vec0, cell0);
    vec0 = vector3(3,3,3);
    cell0.i = 2333;
    space_hashtable_set(hash, vec0, cell0);

    vec1 = vector3(0,0,0);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 2000 == cell1.i);
    vec1 = vector3(1,0,0);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 2100 == cell1.i);
    vec1 = vector3(2,0,0);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 2200 == cell1.i);
    vec1 = vector3(3,0,0);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 2300 == cell1.i);
    vec1 = vector3(3,3,3);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 2333 == cell1.i);

    // Overwrite some more things
    vec0 = vector3(0,0,0);
    cell0.i = 4000;
    space_hashtable_set(hash, vec0, cell0);
    vec0 = vector3(1,0,0);
    cell0.i = 4100;
    space_hashtable_set(hash, vec0, cell0);
    vec0 = vector3(2,0,0);
    cell0.i = 4200;
    space_hashtable_set(hash, vec0, cell0);
    vec0 = vector3(3,0,0);
    cell0.i = 4300;
    space_hashtable_set(hash, vec0, cell0);
    vec0 = vector3(3,3,3);
    cell0.i = 4333;
    space_hashtable_set(hash, vec0, cell0);

    vec1 = vector3(0,0,0);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 4000 == cell1.i);
    vec1 = vector3(1,0,0);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 4100 == cell1.i);
    vec1 = vector3(2,0,0);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 4200 == cell1.i);
    vec1 = vector3(3,0,0);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 4300 == cell1.i);
    vec1 = vector3(3,3,3);
    cell1 = space_hashtable_get(hash, vec1);
    mu_assert(_FL "bad cell value", 4333 == cell1.i);

    space_hashtable_destroy(hash);
    return 0;
}

static const char *test_space_hashtable_set_get_large() {
    static const uint64_t LARGE = (1<<17);
    cell_t cells[LARGE];
    cell_t cell;
    vector3_t vectors[LARGE];
    space_hashtable_t *hash = NULL;
    uint64_t i;
    srand((unsigned int)time(NULL));

    hash = space_hashtable_create(LARGE>>1);

    // Set lots of cells
    for (i = 0; i < LARGE; ++i) {
        cells[i].i = rand();
        vectors[i] = vector3(rand(),rand(),rand());
        space_hashtable_set(hash, vectors[i], cells[i]);
    }
    // Check lots of cells
    for (i = 0; i < LARGE; ++i) {
        cell = space_hashtable_get(hash, vectors[i]);
        if (cell.i != cells[i].i) printf("Cell value mismatch @ i = %llu, cell.i = %llu != cells[i].i = %llu\n", i, cell.i, cells[i].i);
        mu_assert(_FL "bad cell value from vectors", cell.i == cells[i].i);
    }

    space_hashtable_destroy(hash);
    return 0;
}

static const char *all_tests() {
    mu_run_test(test_space_hashtable_create_destroy);
    mu_run_test(test_space_hashtable_set_get_small);
    mu_run_test(test_space_hashtable_set_get_large);
    return 0;
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    const char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf(__FILE__": ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    return result != 0;
}
