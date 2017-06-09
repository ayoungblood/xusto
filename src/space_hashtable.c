/* space_hashtable.c
 * Hash table implementation for execution space extension
 * Keys are vector3_t, values are cell_t
 */

#include "space_hashtable.h"

/* Create and initialize a new hashtable.
 * size - Number of entries to initialize the hashtable with, must be a power of two
 * Returns a pointer to the hashtable if succesfully initalized, NULL otherwise */
space_hashtable_t *space_hashtable_create(unsigned long long size) {
    space_hashtable_t *hashtable = NULL;
    unsigned long long i;
    if ((size == 0) || (size & (size - 1))) {
        printf("size (%llu) must be a power of two >= 1\n", size);
        return NULL;
    }
    // Allocate the table
    if ((hashtable = malloc(sizeof(space_hashtable_t))) == NULL) {
        return NULL;
    }
    // Allocate pointers to head nodes
    if ((hashtable->table = malloc(sizeof(space_hashtable_entry_t*)*size)) == NULL) {
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
    unsigned long long i;
    space_hashtable_entry_t *next = NULL, *orphan = NULL;
    for (i = 0; i < hashtable->size; ++i) {
        //printf("attempting to destroy hashtable->table[%llu]\n", i);
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
unsigned long long space_hashtable_hash(space_hashtable_t *hashtable, vector3_t key) {
    // Unsigned long will be at least 32 bits. Need a single integer to hash, build it from vector3
    // 24 bits come from x, y, and 16 from z, then we hash the integer as usual
    unsigned long long hash = (unsigned long long)((key.x & 0xffffff) | (key.y & 0xffffff)<<24 | (key.z & 0xffff)<<48);
    printf("%s: composited hash = 0x%016llx\n", __func__, hash);
    // Hash function from https://stackoverflow.com/a/12996028/
    hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9ULL;
    hash = (hash ^ (hash >> 27)) * 0x94d049bb133111ebULL;
    hash = hash ^ (hash >> 31);
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
    printf("%s: hashtable = %p, key = (%lld, %lld, %lld), val.i = %lld\n",
        __func__, (void *)hashtable, (long long)key.x, (long long)key.y, (long long)key.z, (long long)val.i);
    unsigned long long index = 0;
    space_hashtable_entry_t *new = NULL;
    space_hashtable_entry_t *next = NULL;
    space_hashtable_entry_t *last = NULL;
    index = space_hashtable_hash(hashtable, key);
    printf("%s: key = (%lld, %lld, %lld), index = %llu\n",
        __func__, (long long)key.x, (long long)key.z, (long long)key.z, index);
    next = hashtable->table[index];
    printf("%s: next = %p\n", __func__, (void *)next);
    if (next == NULL) {
        // First time inserting at this index
        printf("%s: First time inserting at index %llu\n", __func__, index);
        new = space_hashtable_new(key, val);
        hashtable->table[index] = new;
    } else {
        while (next != NULL && vector3_cmp(key, next->key) != 0) {
            printf("%s: searching, next = %p\n", __func__, (void *)next);
            last = next;
            next = next->next;
        }
        if (next != NULL && vector3_cmp(key, next->key) == 0) {
            /* Key exists, replace the value */
            printf("%s: key exists, replacing value\n", __func__);
            next->val = val;
        } else {
            /* Key doesn't exist, add a new key/value pair */
            new = space_hashtable_new(key, val);
            if (new == NULL) {
                printf("%s: could not get new key/value pair\n",__func__);
                return;
            }
            printf("%s: key doesn't exist, new = %p\n", __func__, (void *)new);
            if (next == NULL) {
                last->next = new;
            } else {
                printf("%s: next is not NULL, inserting\n", __func__);
                new->next = next;
                hashtable->table[index] = new;
            }
        }
    }
}
/* Retrieve a value from the hashtable */
cell_t space_hashtable_get(space_hashtable_t *hashtable, vector3_t key) {
    printf("%s: hashtable = %p, key = (%lld, %lld, %lld)\n",
        __func__, (void *)hashtable, (long long)key.x, (long long)key.y, (long long)key.z);
    unsigned long long index = 0;
    space_hashtable_entry_t *next = NULL;
    index = space_hashtable_hash(hashtable, key);
    printf("%s: key = (%lld, %lld, %lld), index = %llu\n",
        __func__, (long long)key.x, (long long)key.z, (long long)key.z, index);
    next = hashtable->table[index];
    printf("%s: next = %p\n", __func__, (void *)next);
    while (next != NULL && vector3_cmp(key, next->key) != 0) {
        printf("%s: searching, next = %p\n", __func__, (void *)next);
        next = next->next;
    }
    if (next == NULL || vector3_cmp(key, next->key) != 0) {
        return (cell_t){.i = 0};
    } else {
        return next->val;
    }
}
