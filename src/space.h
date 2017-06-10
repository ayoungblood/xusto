/* space.h
 * The execution space type, represents a 3D matrix of cells
 */

#ifndef _SPACE_H
#define _SPACE_H

#include "types.h"
#include "space_hashtable.h"

// Execution space type
typedef struct space_t {
    cell_t ***block; // 3D array of cells
    vector3_t block_size;
    vector3_t block_offset;
    space_hashtable_t *hash;
} space_t;

/* Create and initialize a new space */
space_t *space_create(vector3_t block_size, unsigned long long hash_size);
/* Deallocate and destroy an existing space */
void space_destroy(space_t *space);
/* Set a cell in the space */
void space_set(space_t *space, vector3_t index, cell_t val);
/* Get a cell in the space */
cell_t space_get(space_t *space, vector3_t index);

#endif /* _SPACE_H */
