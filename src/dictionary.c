#include "dscl/dictionary.h"
#include "dscl/hash.h"
#include "dscl/hashmap.h"
#include <string.h>
#include <stdlib.h>

size_t string_hash(const void* key) {
    return (size_t)dscl_fnv32a_hash((const char*)key);
}

int string_equals(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b) == 0;
}

void* string_copy(const void* key) {
    const char* str = key;
    char* copy = malloc(strlen(str) + 1);
    if (copy) {
        strcpy(copy, str);
    }
    return copy;
}

void string_destroy(void* key) {
    free(key);
}

dscl_dictionary_t* dscl_dictionary_create(void) {
    return dscl_hashmap_create(
        64,
        string_hash,
        string_equals,
        string_copy,
        string_destroy
    );
}

void dscl_dictionary_free(dscl_dictionary_t* d) {
    dscl_hashmap_free(d);
}

void* dscl_dictionary_insert(dscl_dictionary_t* d, char* key, void* value) {
    return dscl_hashmap_insert(d, (void*)key, value);
}

void* dscl_dictionary_get(dscl_dictionary_t* d, char* key) {
    return dscl_hashmap_get(d, (void*)key);
}

int dscl_dictionary_remove(dscl_dictionary_t* d, char* key) {
    return dscl_hashmap_remove(d, (void*)key);
}

