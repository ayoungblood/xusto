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

static const char *test_space_set_get_small() {
    return 0;
}

static const char *all_tests() {
    mu_run_test(test_space_create_destroy);
    mu_run_test(test_space_set_get_small);
    //mu_run_test(test_space_set_get_large);
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
