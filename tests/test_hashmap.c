#include "dscl/hashmap.h"
#include "dscl/hash.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==== Callbacks for string keys ====
size_t string_hash(const void* key) {
    return (size_t)dscl_fnv32a_hash(key);
}

int string_equals(const void* a, const void* b) {
    return strcmp(a, b) == 0;
}

void* string_copy(const void* key) {
    const char* str = key;
    char* copy = malloc(strlen(str) + 1);
    strcpy(copy, str);
    return copy;
}

void string_destroy(void* key) {
    free(key);
}

// === Callbacks for integer keys ====
size_t int_hash(const void* key) {
    return (size_t)(*(const int*)key);
}

int int_equals(const void* a, const void* b) {
    return *(const int*)a == *(const int*)b;
}

void* int_copy(const void* key) {
    int* new_key = malloc(sizeof(int));
    if (!new_key) return NULL;
    *new_key = *(const int*)key;
    return new_key;
}

void int_destroy(void* key) {
    free(key);
}

// ==== Test helpers ====
static void test_basic_insert_get(void) {
    dscl_hashmap_t* hm = dscl_hashmap_create(
        4, string_hash, string_equals, string_copy, string_destroy
    );

    char* key1 = "alpha";
    char* val1 = "one";
    assert(dscl_hashmap_insert(hm, key1, val1) != NULL);
    assert(strcmp(dscl_hashmap_get(hm, key1), val1) == 0);
    assert(dscl_hashmap_size(hm) == 1);

    // Update same key
    char* val1b = "uno";
    dscl_hashmap_insert(hm, key1, val1b);
    assert(strcmp(dscl_hashmap_get(hm, key1), val1b) == 0);
    assert(dscl_hashmap_size(hm) == 1);

    dscl_hashmap_free(hm);
}

static void test_remove(void) {
    dscl_hashmap_t* hm = dscl_hashmap_create(
        4, string_hash, string_equals, string_copy, string_destroy
    );

    char* key = "beta";
    char* val = "two";
    dscl_hashmap_insert(hm, key, val);
    assert(dscl_hashmap_size(hm) == 1);

    assert(dscl_hashmap_remove(hm, key) == 0);
    assert(dscl_hashmap_size(hm) == 0);
    assert(dscl_hashmap_get(hm, key) == NULL);

    // Removing again should fail
    assert(dscl_hashmap_remove(hm, key) == 1);

    dscl_hashmap_free(hm);
}

static void test_collisions(void) {
    // Use small capacity to force collisions
    dscl_hashmap_t* hm = dscl_hashmap_create(
        2, string_hash, string_equals, string_copy, string_destroy
    );

    dscl_hashmap_insert(hm, "aa", "first");
    dscl_hashmap_insert(hm, "bb", "second");
    dscl_hashmap_insert(hm, "cc", "third");

    assert(strcmp(dscl_hashmap_get(hm, "aa"), "first") == 0);
    assert(strcmp(dscl_hashmap_get(hm, "bb"), "second") == 0);
    assert(strcmp(dscl_hashmap_get(hm, "cc"), "third") == 0);

    // Remove first, ensure others still work
    dscl_hashmap_remove(hm, "aa");
    assert(strcmp(dscl_hashmap_get(hm, "bb"), "second") == 0);
    assert(strcmp(dscl_hashmap_get(hm, "cc"), "third") == 0);

    dscl_hashmap_free(hm);
}

static void test_expansion(void) {
    dscl_hashmap_t* hm = dscl_hashmap_create(
        2, string_hash, string_equals, string_copy, string_destroy
    );

    char* values[20];

    for (int i = 0; i < 20; i++) {
        char key[16];
        sprintf(key, "k%d", i);
        char* value = malloc(16);
        sprintf(value, "v%d", i);
        values[i] = value;
        dscl_hashmap_insert(hm, key, value);
    }

    for (int i = 0; i < 20; i++) {
        char key[16];
        sprintf(key, "k%d", i);
        char* value = dscl_hashmap_get(hm, key);
        assert(value != NULL);
    }

    dscl_hashmap_free(hm);

    for (int i = 0; i < 20; i++) {
        free(values[i]);
    }
}

static void test_edge_cases(void) {
    dscl_hashmap_t* hm = dscl_hashmap_create(
        4, string_hash, string_equals, string_copy, string_destroy
    );

    // NULL key fails
    assert(dscl_hashmap_get(hm, NULL) == NULL);
    assert(dscl_hashmap_insert(hm, NULL, "value") == NULL);

    // NULL value succeeds
    assert(strcmp(dscl_hashmap_insert(hm, "zeta", NULL), "zeta") == 0);
    assert(dscl_hashmap_has_key(hm, "zeta") == 1);
    assert(dscl_hashmap_has_key(hm, "nonexistent") == 0);

    dscl_hashmap_free(hm);
}

static void test_int_keys(void) {
    dscl_hashmap_t* hm = dscl_hashmap_create(
        8, int_hash, int_equals, int_copy, int_destroy
    );

    // Insert integers
    for (int i = 0; i < 100; i++) {
        int value = i * 10;
        dscl_hashmap_insert(hm, &i, (void*)(intptr_t)value);
    }

    // Verify values
    for (int i = 0; i < 100; i++) {
        int got = (int)(intptr_t)dscl_hashmap_get(hm, &i);
        assert(got == i * 10);
    }

    // Remove half
    for (int i = 0; i < 50; i++) {
        assert(dscl_hashmap_remove(hm, &i) == 0);
    }

    // Ensure removed keys are gone
    for (int i = 0; i < 50; i++) {
        assert(dscl_hashmap_get(hm, &i) == NULL);
    }

    // Ensure remaining keys are intact
    for (int i = 50; i < 100; i++) {
        int got = (int)(intptr_t)dscl_hashmap_get(hm, &i);
        assert(got == i * 10);
    }

    dscl_hashmap_free(hm);
}

int main(void) {
    test_basic_insert_get();
    test_remove();
    test_collisions();
    test_expansion();
    test_edge_cases();
    test_int_keys();

    printf("All hashmap regression tests passed.\n");
    return 0;
}
