// font_support_checker.c
// Compile and run with
//    gcc -Wall -Wextra -o font ../src/types.c ../src/util.c font_support_checker.c && ./font && rm font

#include "../src/types.h"
#include "../src/util.h"

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    xint_t c;
    for (c = 0x1D400; c < 0x1EE00; c += 0x10) {
        printf("U+%llX: ", (unsigned long long)c);
        print_utf8(c);
        printf("\n");
    }

    return 0;
}
