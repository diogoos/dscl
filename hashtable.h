#pragma once
#include<stdlib.h>

#ifndef HASHTABLE_LIBRARY_H
#define HASHTABLE_LIBRARY_H

// Definition of the hash table
typedef struct hashtable_t hashtable_t;

size_t hash_key(const char* str, const size_t p, const size_t m);

// Methods to create and delete the hash table
hashtable_t* create_hashtable(size_t capacity);
void destroy_hashtable(hashtable_t* ht);

void* hashtable_get(hashtable_t* ht, const char* key);
const char* hashtable_insert(hashtable_t* ht, const char* key, void* value);
int hashtable_delete(hashtable_t* ht, const char* key);
int hashtable_has_key(hashtable_t* ht, const char* key);

void hashtable_dump(hashtable_t* ht);

#endif //HASHTABLE_LIBRARY_H