/* types.h
 * Types for just about everything
 */

#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>

typedef uint32_t flags_t;

typedef struct fp_list_t {
    int length;
    FILE **files;
    char **filenames;
} fp_list_t;

#endif /* _TYPES_H */
