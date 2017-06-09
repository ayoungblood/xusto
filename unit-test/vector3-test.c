/* src/vector3-test.c
 * Very simple unit tests for vector3_t from src/types.h
 */

#include "minunit.h"
#include "../src/types.h"

int tests_run = 0;

static const char *test_vector3() {
    vector3_t a = {111,222,333};
    a.x *= -1;
    a.z *= -2;
    mu_assert(_FL "bad vector value", a.x == -111);
    mu_assert(_FL "bad vector value", a.y == 222);
    mu_assert(_FL "bad vector value", a.z == -666);
    vector3_t b = vector3(1,2,3);
    mu_assert(_FL "bad vector value", b.x == 1);
    mu_assert(_FL "bad vector value", b.y == 2);
    mu_assert(_FL "bad vector value", b.z == 3);
    a = b;
    mu_assert(_FL "bad vector value", a.x == 1);
    mu_assert(_FL "bad vector value", a.y == 2);
    mu_assert(_FL "bad vector value", a.z == 3);
    a = vector3(0xf00fdeadbeef,0xf11fdeadbeef,0xf22fdeadbeef);
    mu_assert(_FL "bad vector value", a.x == 0xf00fdeadbeef);
    mu_assert(_FL "bad vector value", a.y == 0xf11fdeadbeef);
    mu_assert(_FL "bad vector value", a.z == 0xf22fdeadbeef);
    return 0;
}

static const char *test_vector3_cmp() {
    vector3_t a, b;
    a = vector3(1,2,3);
    b = vector3(1,2,3);
    mu_assert(_FL "vectors should be equal", vector3_cmp(a,b) == 0);
    a = vector3(0xf00fdeadbeef,0xf11fdeadbeef,0xf22fdeadbeef);
    b = vector3(0xf00fdeadbeef,0xf11fdeadbeef,0xf22fdeadbeef);
    mu_assert(_FL "vectors should be equal", vector3_cmp(a,b) == 0);
    a = vector3(0,0,1);
    b = vector3(0,1,0);
    mu_assert(_FL "vectors should not be equal", vector3_cmp(a,b) != 0);
    a = vector3(-1,0,0);
    b = vector3(0xffffffff,0,0);
    mu_assert(_FL "vectors should not be equal", vector3_cmp(a,b) != 0);
    return 0;
}

static const char *test_vector3_addv() {
    vector3_t a, b, c, d, z = vector3(0,0,0);
    a = vector3(1,2,3);
    b = vector3(-1,-2,-3);
    c = vector3_addv(a,b);
    mu_assert(_FL "vectors should be equal", vector3_cmp(c,z) == 0);
    a = vector3(11,22,33);
    b = vector3(11,22,33);
    c = vector3_addv(a,b);
    d = vector3(22,44,66);
    mu_assert(_FL "vectors should be equal", vector3_cmp(c,d) == 0);
    a = vector3(-22,-44,-66);
    c = vector3_addv(a,z);
    mu_assert(_FL "vectors should be equal", vector3_cmp(c,a) == 0);
    return 0;
}

static const char *all_tests() {
    mu_run_test(test_vector3);
    mu_run_test(test_vector3_cmp);
    mu_run_test(test_vector3_addv);
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
