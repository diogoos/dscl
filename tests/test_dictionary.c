//
// Created by Diogo Silva on 2/5/25.
//

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "dscl/dictionary.h"

#define NUM_TEST_KEYS 500

int test_hardcoded_lookup(void) {
    printf("Testing hardcoded insert and retrieve...\n");

    char* key1 = "key1";
    char* key2 = "key2";
    char* key3 = "key3";
    char* key4 = "key4";
    char* key5 = "key5";

    char* value1 = "value1";
    char* value2 = "value2";
    char* value3 = "value3";
    char* value4 = "value4";
    char* value5 = "value5";

    dscl_dictionary_t* ht = dscl_dictionary_create();
    dscl_dictionary_insert(ht, key1, (void*)value1);
    dscl_dictionary_insert(ht, key2, (void*)value2);
    dscl_dictionary_insert(ht, key3, (void*)value3);
    dscl_dictionary_insert(ht, key4, (void*)value4);
    dscl_dictionary_insert(ht, key5, (void*)value5);

    char* result1 = dscl_dictionary_get(ht, key1);
    char* result2 = dscl_dictionary_get(ht, key2);
    char* result3 = dscl_dictionary_get(ht, key3);
    char* result4 = dscl_dictionary_get(ht, key4);
    char* result5 = dscl_dictionary_get(ht, key5);

    assert(strcmp(value1, result1) == 0);
    assert(strcmp(value2, result2) == 0);
    assert(strcmp(value3, result3) == 0);
    assert(strcmp(value4, result4) == 0);
    assert(strcmp(value5, result5) == 0);

    // Now, retest after a few removes
    dscl_dictionary_remove(ht, key3);
    dscl_dictionary_remove(ht, key4);

    result1 = dscl_dictionary_get(ht, key1);
    result2 = dscl_dictionary_get(ht, key2);
    result3 = dscl_dictionary_get(ht, key3);
    result4 = dscl_dictionary_get(ht, key4);
    result5 = dscl_dictionary_get(ht, key5);

    assert(strcmp(value1, result1) == 0);
    assert(strcmp(value2, result2) == 0);
    assert(result3 == NULL);
    assert(result4 == NULL);
    assert(strcmp(value5, result5) == 0);

    dscl_dictionary_free(ht);

    printf("Simple insert and retrieve test passed.\n");
    return 0;
}


void test_insert_get(void) {
    printf("Testing insert, get, and delete operations...\n");
    dscl_dictionary_t *ht = dscl_dictionary_create();

    // Allocate memory for keys and values
    char (*keys)[20] = malloc(NUM_TEST_KEYS * sizeof(*keys));
    int *values = malloc(NUM_TEST_KEYS * sizeof(int));
    assert(keys != NULL && values != NULL);

    // Insert keys and values
    for (int i = 0; i < NUM_TEST_KEYS; i++) {
        snprintf(keys[i], 20, "%d", i);  // Store key as a string
        values[i] = i;
        dscl_dictionary_insert(ht, keys[i], &values[i]);
    }

    // Get and validate values
    for (int i = 0; i < NUM_TEST_KEYS; i++) {
        int* value = dscl_dictionary_get(ht, keys[i]);
        assert(value != NULL && *value == i);
    }

    // Remove half of the values
    for (int i = 0; i < NUM_TEST_KEYS / 2; i++) {
        assert(dscl_dictionary_remove(ht, keys[i]) == 0);
    }

    // Try to remove a non-existent key (already removed)
    assert(dscl_dictionary_remove(ht, keys[0]) == 1);

    // Ensure the values remain correct
    for (int i = NUM_TEST_KEYS / 2; i < NUM_TEST_KEYS; i++) {
        int* value = dscl_dictionary_get(ht, keys[i]);
        assert(value != NULL && *value == i);
    }

    // Cleanup
    free(keys);
    free(values);
    dscl_dictionary_free(ht);
    printf("Insert, get, and delete operations test passed.\n");
}


int main(void) {
    test_hardcoded_lookup();
    test_insert_get();
}
