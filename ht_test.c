//
// Created by Diogo Silva on 2/5/25.
//

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "hashtable.h"


#define NUM_ENTRIES 50000  // Adjust based on desired test size

int speed_test() {
    hashtable_t* ht = create_hashtable(NUM_ENTRIES*2);
    if (!ht) {
        fprintf(stderr, "Failed to create hash table\n");
        return 1;
    }

    char key[32];
    int values[NUM_ENTRIES];

    clock_t start, end;
    double time_taken;

    // Insert test
    start = clock();
    for (size_t i = 0; i < NUM_ENTRIES; i++) {
        snprintf(key, sizeof(key), "key%zu", i);
        hashtable_insert(ht, key, NULL);
    }
    end = clock();
    time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Insertions: %d ops/sec\n", (int)(NUM_ENTRIES / time_taken));

    // Lookup test
    start = clock();
    for (size_t i = 0; i < NUM_ENTRIES; i++) {
        snprintf(key, sizeof(key), "key%zu", i);

        // int* result = hashtable_get(ht, key);
        // assert(result == &values[i]);
    }
    end = clock();
    time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Lookups: %d ops/sec\n", (int)(NUM_ENTRIES / time_taken));

    destroy_hashtable(ht);
    return 0;
}


int main() {
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

    hashtable_t* ht = create_hashtable(5);
    hashtable_insert(ht, key1, value1);
    hashtable_insert(ht, key2, value2);
    hashtable_insert(ht, key3, value3);
    hashtable_insert(ht, key4, value4);
    hashtable_insert(ht, key5, value5);

    hashtable_dump(ht);


    char* result1 = hashtable_get(ht, key1);
    char* result2 = hashtable_get(ht, key2);
    char* result3 = hashtable_get(ht, key3);
    char* result4 = hashtable_get(ht, key4);
    char* result5 = hashtable_get(ht, key5);

    printf("key: %s; value: %s; result: %s;\n", key1, value1, result1);
    printf("key: %s; value: %s; result: %s;\n", key2, value2, result2);
    printf("key: %s; value: %s; result: %s;\n", key3, value3, result3);
    printf("key: %s; value: %s; result: %s;\n", key4, value4, result4);
    printf("key: %s; value: %s; result: %s;\n", key5, value5, result5);

    speed_test();
}