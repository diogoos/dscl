#include <stdio.h>
#include <time.h>
#include "dscl/dictionary.h"

#define NUM_TEST_KEYS 5000

int main(void) {
    printf("Benchmarking hashmap...\n");
    dscl_dictionary_t* ht = dscl_dictionary_create();
    if (!ht) {
        fprintf(stderr, "Failed to create hashmap\n");
        return 1;
    }

    char key[32];

    // Insert speed test
    clock_t start = clock();
    for (size_t i = 0; i < NUM_TEST_KEYS; i++) {
        snprintf(key, sizeof(key), "key%zu", i);
        dscl_dictionary_insert(ht, key, NULL);
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

    // Cleanup
    dscl_dictionary_free(ht);
    return 0;
}
