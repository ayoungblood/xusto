/* space_hashtable.c
 * Hash table implementation for execution space extension
 * Keys are vector3_t, values are cell_t
 */

#include "space_hashtable.h"

/* Create and initialize a new hashtable.
 * size - Number of entries to initialize the hashtable with, must be a power of two
 * Returns a pointer to the hashtable if succesfully initalized, NULL otherwise */
space_hashtable_t *space_hashtable_create(size_t size) {
    space_hashtable_t *hashtable = NULL;
    size_t i;
    if ((size == 0) || (size & (size - 1))) {
        eprintf("%s: size (%zu) must be a power of two >= 1\n", __func__, size);
        return NULL;
    }
    // Allocate the table
    if ((hashtable = malloc(sizeof(space_hashtable_t))) == NULL) {
        eprintf("%s: Failed to allocate hashtable\n", __func__);
        return NULL;
    }
    // Allocate pointers to head nodes
    if ((hashtable->table = malloc(sizeof(space_hashtable_entry_t*)*size)) == NULL) {
        eprintf("%s: Failed to allocate hashtable entries\n", __func__);
        return NULL;
    }
    // Initialize pointers
    for (i = 0; i < size; ++i) {
        hashtable->table[i] = NULL;
    }
    hashtable->size = size;
    return hashtable;
}
/* Deallocate and destroy an existing hashtable */
void space_hashtable_destroy(space_hashtable_t *hashtable) {
    size_t i;
    space_hashtable_entry_t *next = NULL, *orphan = NULL;
    for (i = 0; i < hashtable->size; ++i) {
        //printf("attempting to destroy hashtable->table[%zu]\n", i);
        next = hashtable->table[i];
        while (next != NULL) {
            //printf("%s: searching, next = %p\n", __func__, (void *)next);
            orphan = next;
            next = next->next;
            free(orphan);
        }
    }
    free(hashtable->table);
    free(hashtable);
}
/* Hash a key for a given hashtable */
size_t space_hashtable_hash(space_hashtable_t *hashtable, vector3_t key) {
    // Using size_t for this function isn't great, @TODO add better behaviour for 32-bit arch
    // Unsigned long will be at least 32 bits. Need a single integer to hash, build it from vector3
    // 24 bits come from x, y, and 16 from z, then we hash the integer as usual
    size_t hash = (uint64_t)((key.x & 0xffffff) | (key.y & 0xffffff)<<24 | (key.z & 0xffff)<<48);
    //printf("%s: composited hash = 0x%zx\n", __func__, hash);
    // Hash function from https://stackoverflow.com/a/12996028/
    hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9ULL;
    hash = (hash ^ (hash >> 27)) * 0x94d049bb133111ebULL;
    hash = hash ^ (hash >> 31);
    //printf("%s: hashed hash = 0x%zx, hash result = 0x%zx\n", __func__, hash, hash % hashtable->size);
    // @TODO: speed this up by storing a bit mask and making this a bit AND
    return hash % hashtable->size;
}
/* Create a new key-value pair */
space_hashtable_entry_t *space_hashtable_new(vector3_t key, cell_t val) {
    space_hashtable_entry_t *new;
    if ((new = malloc(sizeof(space_hashtable_entry_t))) == NULL) {
        return NULL;
    }
    new->key = key;
    new->val = val;
    new->next = NULL;
    return new;
}
/* Insert a key-value pair into a hashtable */
void space_hashtable_set(space_hashtable_t *hashtable, vector3_t key, cell_t val) {
    size_t index = 0;
    space_hashtable_entry_t *new = NULL;
    space_hashtable_entry_t *next = NULL;
    space_hashtable_entry_t *last = NULL;
    index = space_hashtable_hash(hashtable, key);
    next = hashtable->table[index];
    if (next == NULL) {
        // First time inserting at this index
        new = space_hashtable_new(key, val);
        hashtable->table[index] = new;
    } else {
        while (next != NULL && vector3_cmp(key, next->key) != 0) {
            last = next;
            next = next->next;
        }
        if (next != NULL && vector3_cmp(key, next->key) == 0) {
            /* Key exists, replace the value */
            next->val = val;
        } else {
            /* Key doesn't exist, add a new key/value pair */
            new = space_hashtable_new(key, val);
            if (new == NULL) {
                eprintf("%s: could not get new key/value pair\n",__func__);
                return;
            }
            if (next == NULL) {
                last->next = new;
            } else {
                new->next = next;
                hashtable->table[index] = new;
            }
        }
    }
}
/* Retrieve a value from the hashtable */
cell_t space_hashtable_get(space_hashtable_t *hashtable, vector3_t key) {
    size_t index = 0;
    space_hashtable_entry_t *next = NULL;
    index = space_hashtable_hash(hashtable, key);
    next = hashtable->table[index];
    while (next != NULL && vector3_cmp(key, next->key) != 0) {
        next = next->next;
    }
    if (next == NULL || vector3_cmp(key, next->key) != 0) {
        return (cell_t){.i = 0};
    } else {
        return next->val;
    }
}
