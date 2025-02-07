cmake_minimum_required(VERSION 3.30)
project(simplelib C)

set(CMAKE_C_STANDARD 11)

add_library(simplelib STATIC hashtable.c hash.c)

# testing binary
add_executable(ht_test ht_test.c)
target_link_libraries(ht_test simplelib)

# enable testing
enable_testing()
add_test(
        NAME ht_test
        COMMAND $<TARGET_FILE:ht_test>
)
