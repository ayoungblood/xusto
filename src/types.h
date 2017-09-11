/* types.h
 * Simple types and helper functions for those types
 */

#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <float.h>
#include <stdlib.h>

// General signed integer type
typedef int_fast64_t    xint_t;
// Format constants for xint_t
#define XId             PRIdFAST64  // signed decimal integer
#define XIx             PRIxFAST64  // unsigned lowercase hex integer
// General floating point type
typedef double          xfloat_t;

/* Integer operations */
/* C doesn't have a modulo operator, add one for xint_t */
xint_t xmod(xint_t a, xint_t b);

/* Union type for a cell representing signed integer or float types */
typedef union {
    xint_t i;
    xfloat_t f;
} cell_t;
/* Syntactic sugar for creating a cell from an integer */
cell_t cell(xint_t i);

/* 3-element vector type */
typedef struct vector3_t {
    xint_t x, y, z;
} vector3_t;
/* Syntactic sugar for creating and setting a vector3 */
vector3_t vector3(xint_t x, xint_t y, xint_t z);
/* Returns 0 if the vectors are equal, non-zero otherwise */
int vector3_cmp(const vector3_t a, const vector3_t b);
/* Adds two vector3 (A + B) */
vector3_t vector3_addv(const vector3_t a, const vector3_t b);
/* Subtracts two vector3 (A - B) */
vector3_t vector3_subv(const vector3_t a, const vector3_t b);
/* Multiplies a vector3 by scalar (A * s) */
vector3_t vector3_muls(const vector3_t a, const xint_t s);
/* Divides a vector3 by a scalar (A / s) */
vector3_t vector3_divs(const vector3_t a, const xint_t s);
/* Element-wise vector3 multiplication (A .* B) */
vector3_t vector3_mule(const vector3_t a, const vector3_t b);
/* Element-wise vector3 division (A ./ B) */
vector3_t vector3_dive(const vector3_t a, const vector3_t b);
/* Rems a vector3 by a scalar (A % s) */
vector3_t vector3_rems(const vector3_t a, const xint_t s);
/* Element-wise vector3 remainder (A .% B) */
vector3_t vector3_reme(const vector3_t a, const vector3_t b);
/* Mods a vector3 by a scalar (A % s) */
vector3_t vector3_mods(const vector3_t a, const xint_t s);
/* Element-wise vector3 modulo (A .% B) */
vector3_t vector3_mode(const vector3_t a, const vector3_t b);

// Options type (used as a bit vector), referred to as options to avoid confusion
// with state flags type below
typedef uint32_t options_t;

// State type (used as a bit vector)
typedef uint64_t state_flags_t;

// FILE pointer list type
typedef struct fp_list_t {
    int length;
    FILE **files;
    char **filepaths;
} fp_list_t;

/* Clean up a list of file pointers */
void fp_list_cleanup(fp_list_t *fp_list);

#endif /* _TYPES_H */
