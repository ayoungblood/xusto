/* space.c
 * The execution space type, represents a 3D matrix of cells
 */

#include "space.h"

/* Create and initialize a new space */
space_t *space_create(vector3_t block_size, size_t hash_size) {
    space_t *space = NULL;
    xint_t k, j, i;
    if ((block_size.x <= 0) || (block_size.y <= 0) || (block_size.z <= 0)) {
        eprintf("All components of block_size must be positive\n");
        return NULL;
    }
    // Allocate the space
    if ((space = malloc(sizeof(space_t))) == NULL) {
        return NULL;
    }
    // Allocate the block and initialize it
    space->block = (cell_t***)malloc((size_t)block_size.z * sizeof(cell_t**));
    for (k = 0; k < block_size.z; ++k) {
        space->block[k] = (cell_t**)malloc((size_t)block_size.y * sizeof(cell_t*));
        for (j = 0; j < block_size.y; ++j) {
            space->block[k][j] = (cell_t*)malloc((size_t)block_size.x * sizeof(cell_t));
            for (i = 0; i < block_size.x; ++i) {
                space->block[k][j][i].i = 0;
            }
        }
    }
    space->block_size = block_size;
    // Default block offset to (0,0,0) for now
    space->block_offset = vector3(0,0,0);
    // Allocate the space_hashtable
    if ((space->hash = space_hashtable_create(hash_size)) == NULL) {
        eprintf("Failed to create space_hashtable\n");
        return NULL;
    }
    return space;
}
/* Deallocate and destroy an existing space */
void space_destroy(space_t *space) {
    xint_t k, j;
    space_hashtable_destroy(space->hash);
    for (k = 0; k < space->block_size.z; ++k) {
        for (j = 0; j < space->block_size.y; ++j) {
            free(space->block[k][j]);
        }
        free(space->block[k]);
    }
    free(space->block);
    free(space);
    space = NULL;
}
/* Set a cell in the space */
void space_set(space_t *space, vector3_t index, cell_t val) {
    if (index.x >= space->block_offset.x && index.x < space->block_size.x + space->block_offset.x &&
        index.y >= space->block_offset.y && index.y < space->block_size.y + space->block_offset.y &&
        index.z >= space->block_offset.z && index.z < space->block_size.z + space->block_offset.z) {
        // Index is inside block, access block directly
        space->block[index.z][index.y][index.x] = val;
    } else {
        // Index is outside block, set a value in the hashtable
        space_hashtable_set(space->hash, index, val);
    }
}
/* Get a cell in the space */
cell_t space_get(space_t *space, vector3_t index) {
    cell_t cell;
    if (index.x >= space->block_offset.x && index.x < space->block_size.x + space->block_offset.x &&
        index.y >= space->block_offset.y && index.y < space->block_size.y + space->block_offset.y &&
        index.z >= space->block_offset.z && index.z < space->block_size.z + space->block_offset.z) {
        // Index is inside block, access block directly
        cell = space->block[index.z][index.y][index.x];
    } else {
        // Index is outside block, get value from the hashtable
        cell = space_hashtable_get(space->hash, index);
    }
    return cell;
}
/* Print a section of the space (prints to stderr) */
void space_print(space_t *space, vector3_t start, vector3_t end, int borders) {
    xint_t k, j, i;
    cell_t cell;
    for (k = start.z; k <= end.z; ++k) {
        eprintf("(%"XId":%"XId",%"XId":%"XId",%"XId"):\n",
            start.x,end.x,start.y,end.y,k);
        if (borders) {
            eprintf("       ");
            for (i = start.x; i <= end.x; ++i) eprintf("%4"XId"", i);
            eprintf("\n       ");
            for (i = start.x; i <= end.x; ++i) eprintf("---- ");
            eprintf("\n");
        }
        for (j = start.y; j <= end.y; ++j) {
            if (borders) eprintf("%4"XId" | ", j);
            for (i = start.x; i <= end.x; ++i) {
                cell = space_get(space, vector3(i,j,k));
                eprintf("%04"XIx" ", cell.i);
            }
            eprintf("\n");
        }
    }
}
