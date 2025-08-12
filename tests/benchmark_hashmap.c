#include <stdio.h>
#include <time.h>
#include "dscl/hashmap.h"

#define NUM_TEST_KEYS 50000

size_t int_hash(const void* key) {
    return (size_t)(*(const int*)key);
}

int int_equals(const void* a, const void* b) {
    return *(const int*)a == *(const int*)b;
}

void* int_copy(const void* key) {
    // Just passthrough the original value,
    // since its not being destroyed / cleaned up
    return (void*)key;
}

void int_destroy(void* key) {
    // We are freeing all memory at the end, 
    // no need to destroy here
}

int main(void) {
    printf("Benchmarking hashmap...\n");
    dscl_hashmap_t* ht = dscl_hashmap_create(
        1024, int_hash, int_equals, int_copy, int_destroy
    );
    if (!ht) {
        fprintf(stderr, "Failed to create hashmap\n");
        return 1;
    }

    // Create keys & values
    int* keys = calloc(NUM_TEST_KEYS, sizeof(int));
    int* values = calloc(NUM_TEST_KEYS, sizeof(int));
    if (!keys || !values) {
        fprintf(stderr, "Unable to allocate test keys\n");
        return 1;
    }
    for (int i = 0; i < NUM_TEST_KEYS; i++) keys[i] = i;
    for (int i = 0; i < NUM_TEST_KEYS; i++) values[i] = i * 10;

    clock_t start, end;
    double elapsed;

    // Insert benchmark
    start = clock();
    for (int i = 0; i < NUM_TEST_KEYS; i++) {
        dscl_hashmap_insert(ht, &keys[i], (void*)&values[i]);
    }
    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\tInsert: %.2f ops/sec\n", (double)NUM_TEST_KEYS / elapsed);

    // Lookup benchmark
    start = clock();
    for (int i = 0; i < NUM_TEST_KEYS; i++) {
        int* value = dscl_hashmap_get(ht, &keys[i]);
        if (!value || *value != i * 10) {
            fprintf(stderr, "Lookup key error for %d\n", i);
            return 1;
        }
    }
    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\tLookups: %.2f ops/sec\n", (double)NUM_TEST_KEYS / elapsed);

    // Remove benchmark
    start = clock();
    for (int i = 0; i < NUM_TEST_KEYS; i++) {
        dscl_hashmap_remove(ht, &keys[i]);
    }
    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\tRemove: %.2f ops/sec\n", (double)NUM_TEST_KEYS / elapsed);

    // Cleanup
    free(keys);
    free(values);
    dscl_hashmap_free(ht);
    return 0;
}
