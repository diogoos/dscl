#include "dscl/hash.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
    assert(dscl_fnv32a_hash("hello") == 0x4f9f2cab);
    assert(dscl_fnv32a_hash("a1ph4num3ric") == 0xc6c446ae);
    assert(dscl_fnv32a_hash(" ") == 0x250c8f7f);
    assert(dscl_fnv32a_hash("!@#$%^&*()") == 0x408eb4a7);
    assert(dscl_fnv32a_hash("") == 0x811c9dc5);
}

