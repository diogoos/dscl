#include "hash.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
    assert(fnv32a_hash_str("hello") == 0x4f9f2cab);
    assert(fnv32a_hash_str("a1ph4num3ric") == 0xc6c446ae);
    assert(fnv32a_hash_str(" ") == 0x250c8f7f);
    assert(fnv32a_hash_str("!@#$%^&*()") == 0x408eb4a7);
    assert(fnv32a_hash_str("") == 0x811c9dc5);
}

