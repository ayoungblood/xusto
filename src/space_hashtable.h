/* space_hashtable.h
 *
 */

#ifndef _SPACE_HASHTABLE_H
#define _SPACE_HASHTABLE_H

#include <stdlib.h>

#include "types.h"

typedef struct space_hashtable_entry_t {
    vector3_t key;
    cell_t val;
    struct space_hashtable_entry_t *next;
} space_hashtable_entry_t;

typedef struct space_hashtable_t {
    unsigned int size;
    space_hashtable_entry_t **table;
} space_hashtable_t;

/* Create and initialize a new hashtable */
space_hashtable_t *space_hashtable_init(unsigned int size);
/* Deallocate and destroy an existing hashtable */
void space_hashtable_destroy(space_hashtable_t *hashtable);
/* Hash a key for a given hashtable */
unsigned int space_hashtable_hash(space_hashtable_t *hashtable, vector3_t key);
/* Create a new key-value pair */
space_hashtable_entry_t *space_hashtable_new(vector3_t key, cell_t val);
/* Insert a key-value pair into a hashtable */
void space_hashtable_set(space_hashtable_t *hashtable, vector3_t key, cell_t val);
/* Retrieve a value from the hashtable */
cell_t space_hashtable_get(space_hashtable_t *hashtable, vector3_t key);

#endif /* _SPACE_HASHTABLE_H */
