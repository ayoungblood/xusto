/* types.c
 * Helper functions for simple types
 */

#include "types.h"

/* Returns 0 if the vectors are equal, non-zero otherwise */
int vector3_cmp(const vector3_t a, const vector3_t b) {
    if (a.x == b.x && a.y == b.y && a.z == b.z) {
        return 0;
    }
    return 1;
}

/* Syntactic sugar for creating and setting a vector3 */
vector3_t vector3(xint_t x, xint_t y, xint_t z) {
    vector3_t rv;
    rv.x = x;
    rv.y = y;
    rv.z = z;
    return rv;
}
