// vector3-test.c
// Compile and run with
//    gcc -Wall -Wextra -o v3t ../src/types.c vector3_test.c && ./v3t && rm v3t

#include "../src/types.h"

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    vector3_t a;
    a = vector3(11,22,33);
    vector3_t b = a;
    printf("%lld, %lld, %lld\n", a.x, a.y, a.z);
    printf("%lld, %lld, %lld\n", b.x, b.y, b.z);
    printf("%d\n", vector3_cmp(a,b));
    b.z += 1;
    printf("%lld, %lld, %lld\n", a.x, a.y, a.z);
    printf("%lld, %lld, %lld\n", b.x, b.y, b.z);
    printf("%d\n", vector3_cmp(a,b));
    return 0;
}
