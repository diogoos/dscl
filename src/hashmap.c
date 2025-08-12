#include "dscl/hashmap.h"
#include "dscl/hash.h"

#include <stdio.h>
#include <string.h>

#define HT_EXPANSION_FACTOR 0.8
#define HT_EXPANSION_MULTIPLIER 2
#define HT_PRIME 37

/**
* Computes a hash value for a given string `str` by first applying an FNV-1a hash
* and then compressing the result.
*
* @param str The string to be hashed. A null pointer returns 0.
* @param rounds The number of iterations or rounds in hash compression
* @param m The size of the hash table. The result will lie within [0, m-1].
*
* @return A final hash value in the range [0, m-1]. If `str` is NULL, 0 is returned.
**/
size_t hash_key(
    size_t (*hash)(const void* key),
    void* str,
    const size_t rounds,
    const size_t m
) {
    if (str == NULL) return 0;

    size_t const k = hash(str);
    size_t const h1 = k % m;                             // 1st hash compression function
    size_t const h2 = HT_PRIME + (k % (m - HT_PRIME));   // 2nd hash compression function

    return (h1 + (rounds * h2)) % m;
}


// Definition of the hash table and its nodes
enum HashSlotMarker { EMPTY, INSERTED, DELETED };

typedef struct dscl_hashslot {
    void* key;
    void* value;
    enum HashSlotMarker state;
} dscl_hashslot_t;

typedef struct dscl_hashmap {
    dscl_hashslot_t* slots;
    size_t size;
    size_t capacity;

    size_t (*hash)(const void* key);
    int (*equals)(const void* a, const void* b);
    void* (*copy_key)(const void* key);
    void (*destroy_key)(void* key);
} dscl_hashmap_t;

dscl_hashmap_t* dscl_hashmap_create(
    const size_t capacity,
    size_t (*hash)(const void* key),
    int (*equals)(const void* a, const void* b),
    void* (*copy_key)(const void* key),
    void (*destroy_key)(void* key)
) {
    // Allocate a new hash table on the stack
    dscl_hashmap_t* ht = malloc(sizeof(dscl_hashmap_t));
    ht->size = 0;
    ht->capacity = capacity;
    ht->hash = hash;
    ht->equals = equals;
    ht->copy_key = copy_key;
    ht->destroy_key = destroy_key;

    // Allocate the nodes for the initial capacity
    ht->slots = calloc(capacity, sizeof(dscl_hashslot_t));
    if (ht->slots == NULL) {
        free(ht);
        return NULL;
    }
    return ht;
}

void dscl_hashmap_free(dscl_hashmap_t* hm) {
    for (size_t i = 0; i < hm->capacity; i++) {
        hm->destroy_key(hm->slots[i].key);
    }

    free(hm->slots);
    free(hm);
}

typedef enum hmi_result { HMI_INSERT, HMI_REPLACE, HMI_FAIL, HMI_NEEDS_EXPAND } hmi_result_t;

static hmi_result_t dscl_hashtable_slot_insert(dscl_hashslot_t* slots, const size_t capacity,
                                               void* key, void* value, const int cpy, 
                                               dscl_hashmap_t* proto) {
    if (slots == NULL) return HMI_FAIL;

    // Calculate the hashed index of the table
    size_t index = hash_key(proto->hash, key, 0, capacity);

    // Loop through the slots until we find an empty index (or a
    // matching key). It is guaranteed that one of these will occur,
    // since we have expanded the table as needed.
    size_t checks = 0;
    while (slots[index].state != EMPTY) {
        // If a matching key is found at the index, we just need
        // to update its value and we're done!
        if (proto->equals(slots[index].key, key)) {
            slots[index].value = value;
            return HMI_REPLACE;
        }

        // Otherwise, the key wasn't in this slot; we need to move
        // to the next index, as given by the checks
        checks++;
        index = hash_key(proto->hash, key, checks, capacity);

        // if the checks have exceeded the size of the hashtable,
        // there's nowhere else to add, so we need to expand the hashtable
        if (checks >= capacity) {
            return HMI_NEEDS_EXPAND;
        }
    }

    // We found an empty/deleted space, so create a new item at the free slot
    if (cpy == 0) {
        key = proto->copy_key(key);
        if (key == NULL) return HMI_FAIL;
    }

    slots[index].key = (void*)key;
    slots[index].value = value;
    slots[index].state = INSERTED;

    return HMI_INSERT;
}


/**
 * Expands the hash table by reallocating memory for a larger table.
 * Rehashes all existing keys into the new table.
 *
 * @param hm The hash table to be expanded.
 * @return The new capacity of the hash table.
 */
size_t dscl_hashmap_expand(dscl_hashmap_t* hm) {
    size_t const new_capacity = hm->capacity * HT_EXPANSION_MULTIPLIER;
    dscl_hashslot_t* new_slots = calloc(new_capacity, sizeof(dscl_hashslot_t));

    // Rehash all existing keys into the new table
    for (size_t i = 0; i < hm->capacity; i++) {
        dscl_hashslot_t const slot = hm->slots[i];

        if (slot.state == INSERTED) {
            dscl_hashtable_slot_insert(new_slots, new_capacity, slot.key, slot.value, 1, hm);
        }
    }

    // Ensure we're freeing old slots
    free(hm->slots);

    // Update hash table's slots and capacity
    hm->slots = new_slots;
    hm->capacity = new_capacity;

    return new_capacity;
}

void* dscl_hashmap_insert(dscl_hashmap_t* hm, void* key, void* value) {
    if (hm->slots == NULL || key == NULL) return NULL;

    // Check if we first need to expand the table, doing so if needed
    size_t capacity = hm->capacity;
    if ((double)hm->size/hm->capacity >= HT_EXPANSION_FACTOR) {
        capacity = dscl_hashmap_expand(hm);
    }

    hmi_result_t result = dscl_hashtable_slot_insert(hm->slots, capacity, key, value, 0, hm);
    while (result == HMI_NEEDS_EXPAND) {
        capacity = dscl_hashmap_expand(hm);
        result = dscl_hashtable_slot_insert (hm->slots, capacity, key, value, 0, hm);
    }

    switch (result) {
        case HMI_FAIL: return NULL;
        case HMI_INSERT: hm->size++;
        default: break;
    }
    
    return key;
}


int dscl_hashmap_remove(dscl_hashmap_t* hm, void* key) {
    // Calculate the hashed index of the table
    size_t index = hash_key(hm->hash, key, 0, hm->capacity);

    // Loop through the slots until we find an empty index, which
    // would mean that there is no such item in the list
    size_t checks = 0;
    while (hm->slots[index].state != EMPTY && checks < hm->capacity) {
        // If a matching key is found at the index, we should now delete it
        if (hm->slots[index].state == INSERTED && hm->equals(hm->slots[index].key, key)) {
            hm->slots[index].state = DELETED;
            hm->slots[index].value = NULL;
            hm->destroy_key(hm->slots[index].key);
            hm->slots[index].key = NULL;

            hm->size--;
            return 0;
        }

        // Otherwise, the key wasn't in this slot; we need to check
        // the next index, to check a collisions
        checks++;
        index = hash_key(hm->hash, key, checks, hm->capacity);
    }

    // There is no such item in the hash table, so it cannot be deleted
    return 1;
}

void* dscl_hashmap_get(const dscl_hashmap_t* hm, void* key) {
    if (key == NULL) return NULL;

    size_t index = hash_key(hm->hash, key, 0, hm->capacity);
    size_t checks = 0;
    while (hm->slots[index].state != EMPTY && checks < hm->capacity) {
        // If we have a matching key, return it
        if (hm->slots[index].state == INSERTED && hm->equals(hm->slots[index].key, key)) {
            return hm->slots[index].value;
        }

        // Otherwise, check the next possible index
        checks++;
        index = hash_key(hm->hash, key, checks, hm->capacity);
    }

    // There is no such item in the hashtable
    return NULL;
}

int dscl_hashmap_size(dscl_hashmap_t* hm) {
    return hm->size;
}

int dscl_hashmap_has_key(dscl_hashmap_t* hm, void* key) {
    if (key == NULL) return -1;

    size_t index = hash_key(hm->hash, key, 0, hm->capacity);
    size_t checks = 0;
    while (hm->slots[index].state != EMPTY && checks < hm->capacity) {
        // If we have a matching key
        if (hm->slots[index].state == INSERTED && hm->equals(hm->slots[index].key, key)) {
            return 1;
        }

        // Otherwise, check the next possible index
        checks++;
        index = hash_key(hm->hash, key, checks, hm->capacity);
    }

    // There is no such item in the hashtable
    return 0;
}

void dscl_hashmap_debug(const dscl_hashmap_t* hm) {
    printf("\n>>>> hashmap debug <<<<\n");
    printf("* capacity: %zu\n", hm->capacity);
    printf("* size: %zu\n", hm->size);
    printf(">>>>>>>>>>>>>>>>>>>>\n");
    for (int i = 0; i < hm->capacity; i++) {
        printf("[h%3d] ", i);
        dscl_hashslot_t slot = hm->slots[i];
        switch(slot.state) {
            case DELETED:
                printf("DEL    "); break;
            case EMPTY:
                printf("       "); break;
            case INSERTED:
                printf("INS    "); break;
        }
        if (slot.value != NULL) {
            printf("v@%p\t", slot.value);
        } else {
            printf(".               \t");
        }
        
        if (slot.key != NULL) {
            printf("k:'%p'", slot.key);
        } else {
            printf(".");
        }

        printf("\n");
    }
    printf("<<<<<<<<<<<<<<<<<<<<\n\n");
}
