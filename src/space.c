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
    unsigned long long block_size_bytes = (unsigned long long)block_size.x * (unsigned long long)block_size.y * (unsigned long long)block_size.z * sizeof(cell_t);
    printf("Allocating block array, size is %llu bytes\n", block_size_bytes);
    if ((space->block = malloc(block_size_bytes)) == NULL) {
        return NULL;
    }
    space->block_size = block_size;
    // Default block offset to (0,0,0) for now
    space->block_offset = vector3(0,0,0);
    // Allocate the space_hashtable
    if ((space->hash = space_hashtable_create(hash_size)) == NULL) {
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
