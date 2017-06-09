/* src/vector3-test.c
 * Very simple unit tests for vector3_t from src/types.h
 */

//#include <stdio.h>
//#include <stdbool.h>

#include "minunit.h"
#include "../src/types.h"
#include "../src/space_hashtable.h"

int tests_run = 0;

static const char *test_space_hashtable_create_destroy() {
    cell_t cell0, cell1, cell2, cell3;
    vector3_t vec0, vec1;
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
    cell_t cell0, cell1, cell2, cell3;
    vector3_t vec0, vec1;
    space_hashtable_t *hash = NULL;

    return 0;
}

static const char *test_space_hashtable_set_get_large() {
    cell_t cell0, cell1, cell2, cell3;
    vector3_t vec0, vec1;
    space_hashtable_t *hash = NULL;

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
