#ifndef DSCL_HASHMAP_H
#define DSCL_HASHMAP_H

#include <stdlib.h>

typedef struct dscl_hashmap dscl_hashmap_t;

/**
 * Creates a new hash map with the specified capacity.
 * Allocates memory for both the hash map structure and the slots.
 *
 * @param capacity The initial capacity of the hash map.
 * @return A pointer to the created hash map, or NULL if memory allocation fails.
 */
dscl_hashmap_t* dscl_hashmap_create(
    const size_t capacity,
    size_t (*hash)(const void* key),
    int (*equals)(const void* a, const void* b),
    void* (*copy_key)(const void* key),
    void (*destroy_key)(void* key)
);

/**
 * Frees the memory associated with the hash map.
 * This includes freeing both the slots and the hash map structure.
 *
 * @param hm The hash map to be freed.
 */
void dscl_hashmap_free(dscl_hashmap_t* hm);

/**
 * Inserts a key-value pair into the hash map.
 * If the map is full, it will be expanded before inserting.
 *
 * @param hm The hash map where the key-value pair will be inserted.
 * @param key The key to be inserted.
 * @param value The value associated with the key.
 * @return The key that was inserted, or NULL if insertion failed.
 */
void* dscl_hashmap_insert(dscl_hashmap_t* hm, void* key, void *value);

/**
 * Retrieves the value associated with a given key in the hash map.
 * If the key is not found, NULL is returned.
 *
 * @param hm The hash map to search in.
 * @param key The key whose value is to be retrieved.
 * @return The value associated with the key, or NULL if the key is not found.
 */
void* dscl_hashmap_get(const dscl_hashmap_t* hm, void* key);

/**
 * Removes a key-value pair from the hash map.
 * If the key is found, it is marked as deleted, and the size is reduced.
 *
 * @param hm The hash map from which the key-value pair will be removed.
 * @param key The key to be removed.
 * @return 1 if the key was removed successfully, 0 if the key was not found.
 */
int dscl_hashmap_remove(dscl_hashmap_t* hm, void* key);

/**
 * Prints a debug representation of the hash map and its slots
 * to stdout. For debug use only.
 */
void dscl_hashmap_debug(const dscl_hashmap_t* hm);

#endif //DSCL_HASHMAP_H

