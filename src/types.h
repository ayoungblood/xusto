/* types.h
 * Types for just about everything
 */

#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>
#include <stdio.h>
#include <float.h>

// General signed integer type
typedef int_fast64_t xint_t;
// General floating point type
typedef double xfloat_t;
// Union type for a cell
typedef union {
    xint_t i;
    xfloat_t f;
} cell_t;
// 3-element vector type
typedef struct vector3_t {
    xint_t x, y, z;
} vector3_t;
// Execution space type
typedef struct space_t {
    cell_t block[8][256][256];
    vector3_t size;
} space_t;

// Flag type (used as a bit vector)
typedef uint32_t flags_t;

// FILE pointer list type
typedef struct fp_list_t {
    int length;
    FILE **files;
    char **filenames;
} fp_list_t;

#endif /* _TYPES_H */
