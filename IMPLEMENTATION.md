# Implementation
This document provides an in-depth look at the design
and implementation of the components in simplelib. 

## Hash Map (`src/hashmap.c`)
The hash map in simplelib uses **double hashing** for collision resolution,
ensuring a more uniform key distribution and reducing clustering compared
to linear or quadratic probing.

### Design Choices:
- **Collision Resolution:** Double hashing, using a secondary hash function to determine probe steps.
- **Load Factor:** The table resizes when the load factor reaches 0.5, reducing the chance of excessive probing.
- **Resizing Strategy:** When resizing, all elements are rehashed into a table twice the previous size to maintain efficiency.

### Hash Functions:
- **Hash function**: Uses the FNV-1a hash function for fast and efficient key hashing.
- **Primary hash compression:** Uses a simple `m mod n` function 
- **Secondary hash compression:** Offsets the primary hash by using 
a prime `P + (m mod (n - P))` where P is a prime.

### Performance Considerations:
- **Average Case:** Insertions, deletions, and lookups run in O(1) expected time.
- **Worst Case:** In degenerate cases (e.g., poor hash function choice), performance can degrade to O(n).
- **Load Factor:** Helps maintain fast lookups by reducing the number of collisions, at the cost of increased memory usage.

A preliminary test results in:
* Insertions: ~17,200,000 ops/sec
* Retrievals: ~17,500,000 ops/sec

## Red-Black Tree (`src/tree.c`) *(planned)*
The tree module will implements a Red-Black Tree (RBT),
a self-balancing binary search tree.

### Key Properties:
- **Balanced Height:** Guarantees a maximum tree height of O(log n), ensuring efficient insertions, deletions, and lookups.
- **Color-Based Balancing:** Nodes are labeled as red or black, and rotations/recoloring maintain balance after insertions and deletions.

### Performance Considerations:
- Insertions, deletions, and lookups in O(log n) time.
- Self-balancing through rotations (left and right rotations).
