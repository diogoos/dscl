#pragma once
#include<stdlib.h>

#ifndef SIMPLELIB_HASHMAP_H
#define SIMPLELIB_HASHMAP_H

typedef struct HashMap HashMap;

/**
 * Creates a new hash map with the specified capacity.
 * Allocates memory for both the hash map structure and the slots.
 *
 * @param capacity The initial capacity of the hash map.
 * @return A pointer to the created hash map, or NULL if memory allocation fails.
 */
HashMap* hashmap_create(size_t capacity);

/**
 * Frees the memory associated with the hash map.
 * This includes freeing both the slots and the hash map structure.
 *
 * @param hm The hash map to be freed.
 */
void hashmap_free(HashMap* hm);

/**
 * Inserts a key-value pair into the hash map.
 * If the map is full, it will be expanded before inserting.
 *
 * @param hm The hash map where the key-value pair will be inserted.
 * @param key The key to be inserted.
 * @param value The value associated with the key.
 * @return The key that was inserted, or NULL if insertion failed.
 */
const char* hashmap_insert(HashMap* hm, const char *key, void *value);

/**
 * Retrieves the value associated with a given key in the hash map.
 * If the key is not found, NULL is returned.
 *
 * @param hm The hash map to search in.
 * @param key The key whose value is to be retrieved.
 * @return The value associated with the key, or NULL if the key is not found.
 */
void* hashmap_get(const HashMap* hm, const char *key);

/**
 * Removes a key-value pair from the hash map.
 * If the key is found, it is marked as deleted, and the size is reduced.
 *
 * @param hm The hash map from which the key-value pair will be removed.
 * @param key The key to be removed.
 * @return 1 if the key was removed successfully, 0 if the key was not found.
 */
int hashmap_remove(HashMap* hm, const char *key);

#endif //SIMPLELIB_HASHMAP_H