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
    // Allocate the space_hashtable
    if ((space->hash = space_hashtable_create(hash_size)) == NULL) {
        eprintf("Failed to create space_hashtable\n");
        return NULL;
    }
    // No data, so no bounds
    space->bounds = vector3(0,0,0);
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

/* Set a cell in the space and update the bounds to include the cell if necessary */
void space_set_unbounded(space_t *space, vector3_t index, cell_t val) {
    if (index.x < space->block_size.x &&
        index.y < space->block_size.y &&
        index.z < space->block_size.z) {
        // Index is inside block, access block directly
        space->block[index.z][index.y][index.x] = val;
    } else {
        // Index is outside block, set a value in the hashtable
        space_hashtable_set(space->hash, index, val);
    }
    // Update bounds
    if (index.x >= space->bounds.x) space->bounds.x = index.x + 1;
    if (index.y >= space->bounds.y) space->bounds.y = index.y + 1;
    if (index.z >= space->bounds.z) space->bounds.z = index.z + 1;
}
/* Set a cell in the space, modded by bounds */
void space_set(space_t *space, vector3_t index, cell_t val) {
    vector3_t actual = vector3_mode(index, space->bounds);
    if (actual.x < space->block_size.x &&
        actual.y < space->block_size.y &&
        actual.z < space->block_size.z) {
        // Index is inside block, access block directly
        space->block[actual.z][actual.y][actual.x] = val;
    } else {
        // Index is outside block, set a value in the hashtable
        space_hashtable_set(space->hash, actual, val);
    }
    //printf("space_set: (%"XId",%"XId",%"XId") => (%"XId",%"XId",%"XId") = %"XIx"\n",
    //    index.x,index.y,index.z,actual.x,actual.y,actual.z,val.i);
}
/* Get a cell in the space, modded by bounds */
cell_t space_get(space_t *space, vector3_t index) {
    cell_t cell;
    vector3_t actual = vector3_mode(index, space->bounds);

    if (actual.x < space->block_size.x &&
        actual.y < space->block_size.y &&
        actual.z < space->block_size.z) {
        // Index is inside block, access block directly
        cell = space->block[actual.z][actual.y][actual.x];
    } else {
        // Index is outside block, get value from the hashtable
        cell = space_hashtable_get(space->hash, actual);
    }
    //printf("space_get: (%"XId",%"XId",%"XId") => (%"XId",%"XId",%"XId") = %"XIx"\n",
    //    index.x,index.y,index.z,actual.x,actual.y,actual.z,cell.i);
    return cell;
}

/* Set the bounds */
void space_bounds_set(space_t *space, vector3_t bounds) {
    // Do nothing and fail silently if a bounds component is negative
    if (bounds.x < 0 || bounds.y < 0 || bounds.z < 0) return;
    space->bounds = bounds;
}
/* Get the bounds */
vector3_t space_bounds_get(space_t *space) {
    //printf("space_bounds_get: (%"XId",%"XId",%"XId")\n",space->bounds.x,space->bounds.y,space->bounds.z);
    return space->bounds;
}
/* Transform an unbounded coordinate to a coordinate within the space */
vector3_t space_bounds_transform(space_t *space, vector3_t index) {
    return vector3_mode(index,space->bounds);
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
/* Dump the entire space to a file or stream */
void space_dump(space_t *space, FILE *fp, char sep, int opt) {
    xint_t k, j, i;
    cell_t cell;
    (void)opt;
    for (k = 0; k < space->bounds.z; ++k) {
        fprintf(fp,"(%"XIx":%"XIx",%"XIx":%"XIx",%"XIx"):\n",
            0ULL,space->bounds.x-1,0ULL,space->bounds.y-1,k);
        for (j = 0; j < space->bounds.y; ++j) {
            for (i = 0; i < space->bounds.x; ++i) {
                cell = space_get(space, vector3(i,j,k));
                fprintf(fp,"%"XIx"%c", cell.i, sep);
            }
            fprintf(fp,"\n");
        }
        fprintf(fp,"\n");
    }
}
