#include "hashtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HT_EXPANSION_FACTOR 2
#define HT_PRIME 1


// Hashing functions used in the table
#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261U
uint32_t FNV32(const char *s)
{
    uint32_t hash = FNV_OFFSET_32, i;
    for(i = 0; i < strlen(s); i++)
    {
        hash = hash ^ (s[i]); // xor next byte into the bottom of the hash
        hash = hash * FNV_PRIME_32; // Multiply by prime number found to work well
    }
    return hash;
}


size_t h1(size_t const k, const size_t m) {
    return k % m;
}

size_t h2(size_t const k, const size_t m) {
    return 1 + (k % (m - 1));
}

size_t hash_key(const char* str, const size_t p, const size_t m) {
    if (str == NULL) return 0;

    size_t const k = FNV32(str);
    return (h1(k, m) + (p * h2(k, m))) % m;
}


// Definition of the hash table and its nodes
typedef enum HashSlotMarker { EMPTY, INSERTED, DELETED } HashSlotMarker;

typedef struct {
    const char *key;
    void* value;
    enum HashSlotMarker state;
} hashtable_slot;

struct hashtable_t {
    hashtable_slot* slots;
    size_t size;
    size_t capacity;
};


void hashtable_slot_dump(hashtable_slot* slots, size_t capacity) {
    //printf("[capacity: %lu]\n", capacity);
    for (size_t i = 0; i < capacity; i++) {
        //printf("%lu\t%s\t\t%012p\tS%d\t", i, slots[i].key, slots[i].value, slots[i].state);
        // //printf("|%lu|\t", (size_t)(unsigned char *)slots[i].key);
        //printf("A%lu\t", hash_key(slots[i].key, 0, capacity));
        //printf("B%lu\t", hash_key(slots[i].key, 1, capacity));
        //printf("C%lu\t", hash_key(slots[i].key, 2, capacity));
        //printf("\n");
    }
}

void hashtable_dump(hashtable_t* ht) {
    //printf("\nTable size: %lu\nTable capacity: %lu\nTable address: %p\n", ht->size, ht->capacity, ht);
    hashtable_slot_dump(ht->slots, ht->capacity);
    //printf("\n");
}

// Create a new hashtable and allocate the memory as needed
hashtable_t* create_hashtable(const size_t capacity) {
    // Allocate a new hash table on the stack
    hashtable_t* ht = malloc(sizeof(hashtable_t));
    ht->size = 0;
    ht->capacity = capacity;

    // Allocate the nodes for the initial capacity
    ht->slots = calloc(capacity, sizeof(hashtable_slot));
    if (ht->slots == NULL) {
        free(ht);
        return NULL;
    }
    return ht;
}

void destroy_hashtable(hashtable_t* ht) {
    for (size_t i = 0; i < ht->capacity; i++) {
        free((void*)ht->slots[i].key);
    }

    free(ht->slots);
    free(ht);
}

static const char* hashtable_slot_insert(hashtable_slot* slots, const size_t capacity,
                                         const char* key, void* value, const int cpy) {
    if (slots == NULL) return NULL;

    // Calculate the hashed index of the table
    size_t index = hash_key(key, 0, capacity);
    //if (cpy == 0) //printf("[%s got index %lu (p=0;m=%lu)]\n", key, index, capacity);

    // Loop through the slots until we find an empty index (or a
    // matching key). It is guaranteed that one of these will occur,
    // since we have expanded the table as needed.
    size_t checks = 0;
    while (slots[index].state != EMPTY) {
        // If a matching key is found at the index, we just need
        // to update its value and we're done!
        if (strcmp(slots[index].key, key) == 0) {
            slots[index].value = value;
            return slots[index].key;
        }

        // Otherwise, the key wasn't in this slot; we need to move
        // to the next index, as given by the checks
        checks++;
        index = hash_key(key, checks, capacity);
        //if (cpy == 0) //printf("[updated index to %lu]\n", index);
    }

    // We found an empty/deleted space, so create a new item at the free slot
    if (cpy == 0) {
        key = strdup(key);
        if (key == NULL) return NULL;
    }

    slots[index].key = (char*)key;
    slots[index].value = value;
    slots[index].state = INSERTED;

    // if (cpy != 0) {
    hashtable_slot_dump(slots, capacity);
    // }
    return key;
}


// Internal method that expands the hashtable when needed
// However, if we expand the table, we need to recompute the hashes
size_t hashtable_expand(hashtable_t* ht) {
    size_t const new_capacity = ht->capacity * HT_EXPANSION_FACTOR;
    hashtable_slot* new_slots = calloc(new_capacity, sizeof(hashtable_slot));

    // Rehash all existing keys into the new table
    //printf("current slots (%lu): %p\n", ht->capacity, ht->slots);
    for (size_t i = 0; i < ht->capacity; i++) {
        hashtable_slot const slot = ht->slots[i];
        //printf("\tkey: %s\t addr: %p\n", slot.key, slot.value);

        if (slot.state == INSERTED) {
            hashtable_slot_insert(new_slots, new_capacity, slot.key, slot.value, 11);
        }
    }
    //printf("new slots (%lu): %p \n", new_capacity, new_slots);
    for (size_t i = 0; i < new_capacity; i++) {
        hashtable_slot const slot = new_slots[i];
        //printf("\tkey: %s\t addr: %p\n", slot.key, slot.value);
    }
    //printf("\n");

    // Ensure you're freeing old slots
    free(ht->slots);

    // Update hash table's slots and capacity
    ht->slots = new_slots;
    ht->capacity = new_capacity;

    return new_capacity;
}

// Insert a key and value into a hashset
const char* hashtable_insert(hashtable_t* ht, const char* key, void* value) {
    //printf("\n---------------------\nhashtable_insert: key: %s, value: %p\n", key, value);
    if (ht->slots == NULL || value == NULL) return NULL;

    // Check if we first need to expand the table, doing so if needed
    size_t capacity = ht->capacity;
    if (ht->size >= (capacity / HT_EXPANSION_FACTOR)) {
        capacity = hashtable_expand(ht);
    }

    const char* result = hashtable_slot_insert(ht->slots, capacity, key, value, 0);
    if (result == NULL) return NULL;

    ht->size++;
    return result;
}


int hashtable_delete(hashtable_t* ht, const char* key) {
    // Calculate the hashed index of the table
    size_t index = hash_key(key, 0, ht->capacity);

    // Loop through the slots until we find an empty index, which
    // would mean that there is no such item in the list
    size_t checks = 0;
    while (ht->slots[index].state == INSERTED) {
        // If a matching key is found at the index, we should
        // now delete it
        if (strcmp(ht->slots[index].key, key) == 0) {
            ht->slots[index].state = DELETED;
            ht->slots[index].value = NULL;
            ht->slots[index].key = NULL;

            ht->size--;
            return 1;
        }

        // Otherwise, the key wasn't in this slot; we need to check
        // the next index, to check a collisions
        checks++;
        index = hash_key(key, checks, ht->capacity);
    }

    // There is no such item in the hash table, so it cannot be deleted
    return 0;
}

void* hashtable_get(hashtable_t* ht, const char* key) {
    if (key == NULL) return NULL;

    size_t index = hash_key(key, 0, ht->capacity);
    size_t checks = 0;
    while (ht->slots[index].state != EMPTY) {

        // If we have a matching key, return it
        if (strcmp(ht->slots[index].key, key) == 0) {
            return ht->slots[index].value;
        }

        // Otherwise, check the next possible index
        checks++;
        index = hash_key(key, checks, ht->capacity);
    }

    // There is no such item in the hashtable
    return NULL;
}

int hashtable_has_key(hashtable_t* ht, const char* key) {
    return hashtable_get(ht, key) != NULL;
}


