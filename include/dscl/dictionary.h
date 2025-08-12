#ifndef DSCL_DICTIONARY_H
#define DSCL_DICTIONARY_H

#include "dscl/hashmap.h"

typedef dscl_hashmap_t dscl_dictionary_t;

dscl_dictionary_t* dscl_dictionary_create(void);

void dscl_dictionary_free(dscl_dictionary_t* d);

void* dscl_dictionary_insert(dscl_dictionary_t* d, char* key, void* value);

void* dscl_dictionary_get(dscl_dictionary_t* d, char* key);

int dscl_dictionary_remove(dscl_dictionary_t* d, char* key);

#endif // DSCL_DICTIONARY_H
