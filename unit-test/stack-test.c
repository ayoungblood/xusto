/* unit-test/stack-test.c
 * Unit tests for xstack_t from src/xstack.h
 */

#include <stdlib.h>
#include <time.h>

#include "minunit.h"
#include "../src/types.h"
#include "../src/xstack.h"

int tests_run = 0;

static const char *test_stack_create_destroy() {
    static const uint64_t SMALL = (1<<8);
    static const uint64_t LARGE = (1<<12);
    xstack_t *stack = NULL;

    stack = stack_create(SMALL);
    mu_assert(_FL "wrong stack max", stack->max == SMALL);
    stack_destroy(stack);

    stack = stack_create(LARGE);
    mu_assert(_FL "wrong stack max", stack->max == LARGE);
    stack_destroy(stack);

    return 0;
}

static const char *test_stack_push_pop_small() {
    static const uint64_t SIZE = (1<<8);
    xstack_t *stack;
    cell_t cell;
    int i;

    stack = stack_create(SIZE);
    mu_assert(_FL "stack not empty", stack_size(stack) == 0);
    cell = stack_peek(stack);
    mu_assert(_FL "peeking empty stack returned non-zero", cell.i == 0);
    cell = stack_pop(stack);
    mu_assert(_FL "popping empty stack returned non-zero", cell.i == 0);

    cell.i = 666;
    stack_push(stack, cell);
    mu_assert(_FL "push failed", stack_size(stack) == 1);
    cell = stack_peek(stack);
    mu_assert(_FL "bad value from peek", cell.i == 666);
    cell = stack_pop(stack);
    mu_assert(_FL "bad value from pop", cell.i == 666);
    mu_assert(_FL "stack not empty", stack_size(stack) == 0);
    cell = stack_peek(stack);
    mu_assert(_FL "peeking empty stack returned non-zero", cell.i == 0);
    cell = stack_pop(stack);
    mu_assert(_FL "popping empty stack returned non-zero", cell.i == 0);

    for (i = 0; i < 10; ++i) {
        cell.i = i;
        stack_push(stack, cell);
    }
    for (i = 9; i >= 0; --i) {
        cell = stack_pop(stack);
        mu_assert(_FL "bad value from pop", cell.i == i);
    }

    stack_destroy(stack);
    return 0;
}

static const char *all_tests() {
    mu_run_test(test_stack_create_destroy);
    mu_run_test(test_stack_push_pop_small);
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
