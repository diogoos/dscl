cmake_minimum_required(VERSION 3.30)
project(simplelib C)
set(CMAKE_C_STANDARD 11)

include_directories(include)

add_library(simplelib STATIC
        src/hashmap.c
        src/hash.c
)

# add testing executables
add_executable(test_hashmap tests/test_hashmap.c)

# link libraries
target_link_libraries(test_hashmap simplelib)

# enable testing
enable_testing()
add_test(NAME test_hashmap COMMAND $<TARGET_FILE:test_hashmap>)
