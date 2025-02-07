#pragma once
#include<stdlib.h>

#ifndef SIMPLELIB_HASHMAP_H
#define SIMPLELIB_HASHMAP_H

// Definition of the hash table
typedef struct HashMap HashMap;

// Methods to create and delete the hash table
HashMap* hashmap_create(size_t capacity);
void hashmap_free(HashMap *ht);

void* hashmap_get(HashMap *ht, const char *key);
const char* hashmap_insert(HashMap *ht, const char *key, void *value);
int hashmap_remove(HashMap *ht, const char *key);

void hashtable_dump(HashMap *ht);

#endif //SIMPLELIB_HASHMAP_H