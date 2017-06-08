/* types.c
 * Helper functions for simple types
 */

#include "types.h"

/* Syntactic sugar for creating and setting a vector3 */
vector3_t vector3(xint_t x, xint_t y, xint_t z) {
    vector3_t rv;
    rv.x = x;
    rv.y = y;
    rv.z = z;
    return rv;
}

/* Returns 0 if the vectors are equal, non-zero otherwise */
int vector3_cmp(const vector3_t a, const vector3_t b) {
    if (a.x == b.x && a.y == b.y && a.z == b.z) {
        return 0;
    }
    return 1;
}

/* Adds two vector3 (A + B) */
vector3_t vector3_addv(const vector3_t a, const vector3_t b) {
    vector3_t rv;
    rv.x = a.x + b.x;
    rv.y = a.y + b.y;
    rv.z = a.z + b.z;
    return rv;
}
/* Subtracts two vector3 (A - B) */
vector3_t vector3_subv(const vector3_t a, const vector3_t b) {
    vector3_t rv;
    rv.x = a.x - b.x;
    rv.y = a.y - b.y;
    rv.z = a.z - b.z;
    return rv;
}
/* Multiplies a vector3 by scalar (A * s) */
vector3_t vector3_muls(const vector3_t a, const xint_t s) {
    vector3_t rv;
    rv.x = a.x * s;
    rv.y = a.y * s;
    rv.z = a.z * s;
    return rv;
}
/* Divides a vector3 by a scalar (A / s) */
vector3_t vector3_divs(const vector3_t a, const xint_t s) {
    vector3_t rv;
    rv.x = a.x / s;
    rv.y = a.y / s;
    rv.z = a.z / s;
    return rv;
}
/* Element-wise vector3 multiplication (A .* B) */
vector3_t vector3_mule(const vector3_t a, const vector3_t b) {
    vector3_t rv;
    rv.x = a.x * b.x;
    rv.y = a.y * b.y;
    rv.z = a.z * b.z;
    return rv;
}
/* Element-wise vector3 division (A ./ B) */
vector3_t vector3_dive(const vector3_t a, const vector3_t b) {
    vector3_t rv;
    rv.x = a.x / b.x;
    rv.y = a.y / b.y;
    rv.z = a.z / b.z;
    return rv;
}
