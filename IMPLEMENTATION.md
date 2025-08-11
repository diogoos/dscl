# Implementation

## dscl_hashmap
- Use of *double hashing* for collision resolution, ensuring a more uniform key 
distribution and reducing clustering compared to linear or quadratic probing.
- The hash map automatically resizes when the load factor reaches 0.75, reducing 
the chance of excessive probing. When resizing, all elements are rehashed into a 
table twice the previous size.
- Average case insertions, deletions, and lookups run in O(1) expected time. In 
degenerate cases, performance can degrade to O(n).

