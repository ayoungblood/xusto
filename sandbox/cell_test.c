// cell-test.c
// Compile and run with
//    gcc -Wall -Wextra -o ct ../src/types.c cell_test.c && ./ct && rm ct

#include "../src/types.h"

cell_t returns_cell(int x) {
    return (cell_t)(xint_t)x;
}
int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    cell_t cell = {0};
    cell.i += (1<<16);
    printf("%lld, %f\n", cell.i, cell.f);
    cell.f = 1.0;
    printf("%lld, %f\n", cell.i, cell.f);
    cell = returns_cell(0);
    printf("%lld, %f\n", cell.i, cell.f);
    return 0;
}
