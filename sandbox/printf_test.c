// printf_test.c
// Compile and run with
//    gcc -Wall -Wextra -o pt ../src/types.c printf_test.c && ./pt && rm pt

#include "../src/types.h"

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    xint_t a;
    a = 0x2;
    printf("XId = %s\n",XId);
    printf("a = %+08"XId"\n",a);
    printf("a = %04"XIx"\n",a);

    return 0;
}
