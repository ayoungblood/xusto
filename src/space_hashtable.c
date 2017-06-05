/* space_hashtable.c
 * Hash table implementation for execution space extension
 * Keys are vector3_t, values are cell_t
 */

#include "space_hashtable.h"

/* Create and initialize a new hashtable.
 * size - Number of entries to initialize the hashtable with, must be a power of two
 * Returns a pointer to the hashtable if succesfully initalized, NULL otherwise */
space_hashtable_t *space_hashtable_init(unsigned int size) {
    space_hashtable_t *hashtable = NULL;
    unsigned int i;
    if ((size == 0) || (size & (size - 1))) {
        printf("size (%u) must be a power of two >= 1\n", size);
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
    // @TODO FIXME
    (void)hashtable;
    printf("I'm Unimplemented!\n");
}
/* Hash a key for a given hashtable */
unsigned int space_hashtable_hash(space_hashtable_t *hashtable, vector3_t key) {
    // Unsigned long will be at least 32 bits. Need a single integer to hash, build it from vector3
    // 12 bits come from x, y, and 8 from z, then we hash the integer as usual
    unsigned long int hash = (unsigned long)((key.x & 0x3ff) | (key.y & 0x3ff)<<12 | (key.z & 0xff)<<24);
    // Hash function from https://stackoverflow.com/a/12996028/
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash = (hash >> 16) ^ hash;
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
        __func__, (void *)hashtable, (long long)key.x, (long long)key.y, (long long)key.z, val.i);
    unsigned int index = 0;
    space_hashtable_entry_t *new = NULL;
    space_hashtable_entry_t *next = NULL;
    space_hashtable_entry_t *last = NULL;
    index = space_hashtable_hash(hashtable, key);
    printf("%s: key = (%lld, %lld, %lld), index = %u\n",
        __func__, (long long)key.x, (long long)key.z, (long long)key.z, index);
    next = hashtable->table[index];
    printf("%s: next = %p\n", __func__, (void *)next);
    if (next == NULL) {
        // First time inserting at this index
        printf("%s: First time inserting at index %d\n", __func__, index);
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
    unsigned int index = 0;
    space_hashtable_entry_t *next = NULL;
    index = space_hashtable_hash(hashtable, key);
    printf("%s: key = (%lld, %lld, %lld), index = %u\n",
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
