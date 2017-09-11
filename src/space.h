/* space.h
 * The execution space type, represents a 3D matrix of cells
 */

#ifndef _SPACE_H
#define _SPACE_H

#include "util.h"
#include "types.h"
#include "space_hashtable.h"

// Execution space type
typedef struct space_t {
    cell_t ***block; // 3D array of cells
    vector3_t block_size;
    vector3_t bounds;
    space_hashtable_t *hash;
} space_t;

/* Create and initialize a new space */
space_t *space_create(vector3_t block_size, size_t hash_size);
/* Deallocate and destroy an existing space */
void space_destroy(space_t *space);

/* Set a cell in the space and update the bounds to include the cell if necessary */
void space_set_unbounded(space_t *space, vector3_t index, cell_t val);
/* Set a cell in the space, modded by bounds */
void space_set(space_t *space, vector3_t index, cell_t val);
/* Get a cell in the space, modded by bounds */
cell_t space_get(space_t *space, vector3_t index);

/* Set the bounds */
void space_bounds_set(space_t *space, vector3_t bounds);
/* Get the bounds */
vector3_t space_bounds_get(space_t *space);
/* Transform an unbounded coordinate to a coordinate within the space */
vector3_t space_bounds_transform(space_t *space, vector3_t index);

/* Print a section of the space (prints to stderr) */
void space_print(space_t *space, vector3_t start, vector3_t end, int borders);
/* Dump the entire space to a file or stream */
void space_dump(space_t *space, FILE *fp, char sep, int options);

#endif /* _SPACE_H */
