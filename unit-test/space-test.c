/* unit-test/space-test.c
 * Unit tests for space_t from src/space.h
 */

#include <stdlib.h>
#include <time.h>

#include "minunit.h"
#include "../src/types.h"
#include "../src/space_hashtable.h"
#include "../src/space.h"

int tests_run = 0;

static const char *test_space_create_destroy() {
    space_t *space = NULL;
    vector3_t bs;

    bs = vector3(80,80,2);
    space = space_create(bs,(1<<8));
    mu_assert(_FL "incorrect block size", vector3_cmp(bs,space->block_size) == 0);
    space_destroy(space);
    bs = vector3(256,256,8);
    space = space_create(bs,(1<<12));
    mu_assert(_FL "incorrect block size", vector3_cmp(bs,space->block_size) == 0);
    space_destroy(space);
    bs = vector3(4,4,4);
    space = space_create(bs,(1<<16));
    mu_assert(_FL "incorrect block size", vector3_cmp(bs,space->block_size) == 0);
    space_destroy(space);

    return 0;
}

static const char *test_space_set_get_large() {
    static const uint64_t LARGE = (1<<14);
    cell_t cells[LARGE];
    cell_t cell;
    vector3_t vectors[LARGE];
    space_t *space = NULL;
    uint64_t i;
    srand((unsigned int)time(NULL));

    space = space_create(vector3(0x100,0x100,4),LARGE>>4);
    // Set lots of cells all over the place
    // This tests (primarily) the hash access
    for (i = 0; i < LARGE; ++i) {
        cells[i].i = rand();
        // don't randomize x so vectors are unique
        vectors[i] = vector3((signed long long)i - (signed long long)(LARGE>>1),rand(),rand());
        space_set(space, vectors[i], cells[i]);
    }
    // Check cells
    for (i = 0; i < LARGE; ++i) {
        cell = space_get(space, vectors[i]);
        mu_assert(_FL "bad cell value", cell.i == cells[i].i);
    }

    space_destroy(space);
    return 0;
}

static const char *test_space_set_get_large_origin() {
    static const uint64_t M = (1<<14);   // create M unique values..
    static const uint64_t N = (1<<18)-1; // ..ranging from (0..N)
    cell_t cells[M];
    cell_t cell;
    vector3_t vectors[M];
    space_t *space = NULL;
    uint64_t i;

    space = space_create(vector3(0x100,0x100,4),M>>4);
    // Set lots of cells near the origin, using the Knuth method to generate
    // random unique numbers. See https://stackoverflow.com/a/1608585
    // This tests the block access
    uint64_t in, im, unique[M], rn, rm;
    im = 0;
    for (in = 0; in < N && im < M; ++in) {
        rn = N - in;
        rm = M - im;
        if ((uint64_t)rand() % rn < rm)
        unique[im++] = in;
    }
    // unique is now an M-long vector of random, _unique_ numbers, 18 bits long
    // we generate vectors by splitting it as [17:16][15:8][7:0] => zyx
    for (i = 0; i < M; ++i) {
        cells[i].i = rand();
        vectors[i] = vector3(unique[i] & 0xff, (unique[i]>>8) & 0xff, (unique[i]>>16) & 0x3);
        space_set(space, vectors[i], cells[i]);
    }
    // Check cells
    for (i = 0; i < M; ++i) {
        cell = space_get(space, vectors[i]);
        mu_assert(_FL "bad cell value", cell.i == cells[i].i);
    }

    space_destroy(space);
    return 0;
}

static const char *all_tests() {
    mu_run_test(test_space_create_destroy);
    mu_run_test(test_space_set_get_large);
    mu_run_test(test_space_set_get_large_origin);
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
