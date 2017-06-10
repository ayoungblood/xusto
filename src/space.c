/* space.c
 * The execution space type, represents a 3D matrix of cells
 */

#include "space.h"

/* Create and initialize a new space */
space_t *space_create(vector3_t block_size, unsigned long long hash_size) {
    space_t *space = NULL;
    if ((block_size.x <= 0) || (block_size.y <= 0) || (block_size.z <= 0)) {
        printf("All components of block_size must be positive\n");
        return NULL;
    }
    // Allocate the space
    if ((space = malloc(sizeof(space_t))) == NULL) {
        return NULL;
    }
    // Allocate the block
    unsigned long long block_size_bytes = block_size.x * block_size.y * block_size.z * sizeof(cell_t);
    printf("Allocating block array, size is %llu bytes\n", block_size_bytes);
    if ((space.block = malloc(block_size_bytes)) == NULL) {
        return NULL;
    }
    space.block_size = block_size;
    // Default block offset to (0,0,0) for now
    space.block_offset = vector3(0,0,0);
    // Allocate the space_hashtable
    if ((space.hash = space_hashtable_create(hash_size)) == NULL) {
        printf("Failed to create space_hashtable\n");
        return NULL;
    }
    return space;
}
/* Deallocate and destroy an existing space */
void space_destroy(space_t *space) {
    space_hashtable_destroy(space->hash);
    free(space->block);
    free(space);
}
/* Set a cell in the space */
void space_set(space_t *space, vector3_t index, cell_t val) {
    // @TODO FIXME
    return;
}
/* Get a cell in the space */
cell_t space_get(space_t *space, vector3_t index) {
    cell_t cell;
    // @TODO FIXME
    return cell;
}
