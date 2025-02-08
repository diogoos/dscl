//
// Created by Diogo Silva on 2/5/25.
//

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "hashmap.h"

#define NUM_TEST_KEYS 50000

int test_insertion_speed() {
    printf("Testing insertion and lookup speed...\n");

    HashMap* ht = hashmap_create(NUM_TEST_KEYS / 5);
    if (!ht) {
        fprintf(stderr, "Failed to create hash table\n");
        return 1;
    }

    char key[32];

    // Insert speed test
    clock_t start = clock();
    for (size_t i = 0; i < NUM_TEST_KEYS; i++) {
        snprintf(key, sizeof(key), "key%zu", i);
        hashmap_insert(ht, key, NULL);
    }
    clock_t end = clock();
    double time_taken = (double) (end - start) / CLOCKS_PER_SEC;
    printf("\tInsertions: %d ops/sec\n", (int)(NUM_TEST_KEYS / time_taken));

    // Lookup speed test
    start = clock();
    for (size_t i = 0; i < NUM_TEST_KEYS; i++) {
        snprintf(key, sizeof(key), "key%zu", i);
    }
    end = clock();
    time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\tLookups: %d ops/sec\n", (int)(NUM_TEST_KEYS / time_taken));

    // Free the table
    hashmap_free(ht);
    printf("Insertion and lookup speed test completed.\n");
    return 0;
}


int test_hardcoded_lookup() {
    printf("Testing hardcoded insert and retrieve...\n");

    const char* key1 = "key1";
    const char* key2 = "key2";
    const char* key3 = "key3";
    const char* key4 = "key4";
    const char* key5 = "key5";

    const char* value1 = "value1";
    const char* value2 = "value2";
    const char* value3 = "value3";
    const char* value4 = "value4";
    const char* value5 = "value5";

    HashMap* ht = hashmap_create(5);
    hashmap_insert(ht, key1, value1);
    hashmap_insert(ht, key2, value2);
    hashmap_insert(ht, key3, value3);
    hashmap_insert(ht, key4, value4);
    hashmap_insert(ht, key5, value5);

    char* result1 = hashmap_get(ht, key1);
    char* result2 = hashmap_get(ht, key2);
    char* result3 = hashmap_get(ht, key3);
    char* result4 = hashmap_get(ht, key4);
    char* result5 = hashmap_get(ht, key5);

    assert(strcmp(value1, result1) == 0);
    assert(strcmp(value2, result2) == 0);
    assert(strcmp(value3, result3) == 0);
    assert(strcmp(value4, result4) == 0);
    assert(strcmp(value5, result5) == 0);

    // Now, retest after a few removes
    hashmap_remove(ht, key3);
    hashmap_remove(ht, key4);

    result1 = hashmap_get(ht, key1);
    result2 = hashmap_get(ht, key2);
    result3 = hashmap_get(ht, key3);
    result4 = hashmap_get(ht, key4);
    result5 = hashmap_get(ht, key5);

    assert(strcmp(value1, result1) == 0);
    assert(strcmp(value2, result2) == 0);
    assert(result3 == NULL);
    assert(result4 == NULL);
    assert(strcmp(value5, result5) == 0);

    hashmap_free(ht);

    printf("Simple insert and retrieve test passed.\n");
    return 0;
}


void test_insert_get() {
    printf("Testing insert, get, and delete operations...\n");
    HashMap *ht = hashmap_create(NUM_TEST_KEYS);

    // Allocate memory for keys and values
    char (*keys)[20] = malloc(NUM_TEST_KEYS * sizeof(*keys));
    int *values = malloc(NUM_TEST_KEYS * sizeof(int));
    assert(keys != NULL && values != NULL);

    // Insert keys and values
    for (int i = 0; i < NUM_TEST_KEYS; i++) {
        snprintf(keys[i], 20, "%d", i);  // Store key as a string
        values[i] = i;
        hashmap_insert(ht, keys[i], &values[i]);
    }

    // Get and validate values
    for (int i = 0; i < NUM_TEST_KEYS; i++) {
        void* value = hashmap_get(ht, keys[i]);
        assert(value != NULL && *(int*)value == i);
    }

    // Remove half of the values
    for (int i = 0; i < NUM_TEST_KEYS / 2; i++) {
        assert(hashmap_remove(ht, keys[i]) == 0);
    }

    // Try to remove a non-existent key
    assert(hashmap_remove(ht, keys[0]) == 1);

    // Ensure the values remain correct
    for (int i = NUM_TEST_KEYS / 2; i < NUM_TEST_KEYS; i++) {
        void* value = hashmap_get(ht, keys[i]);
        assert(value != NULL && *(int*)value == i);
    }

    // Cleanup
    free(keys);
    free(values);
    hashmap_free(ht);
    printf("Insert, get, and delete operations test passed.\n");
}


int main() {
    test_hardcoded_lookup();
    test_insert_get();
    test_insertion_speed();
}
