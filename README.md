# DSCL - DS Core Library
A lightweight, high-performance C library of fundamental **Data Structures**,
designed as building blocks for future projects.

## Features
- [ ] Hashing & related functions
    - [x] FNV
    - [ ] SHA
    - [ ] HMAC
- [x] Hash maps
    - [x] Generic hashmap
    - [x] Dictionaries
- [ ] Red-black trees
- [ ] BFS and DFS
- [ ] Sorting algorithms

[View implementation details for these features.](./IMPLEMENTATION.md)

## Usage
Include the necessary header files and link `libdscl` during the compilation
of your program. 

To build the library from source, run:
```sh
$ make all
```

To run unit tests, run
```sh
$ make test
```

To run performance benchmarks, run
```sh
$ make benchmark
```

