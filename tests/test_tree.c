#include "dscl/tree.h"
#include <stdlib.h>

int main(void) {
    dscl_tree_t* tree = tree_create(8, NULL);
    tree_insert(tree, 18, NULL);
    tree_insert(tree, 5, NULL);
    tree_insert(tree, 15, NULL);
    tree_insert(tree, 17, NULL);
    tree_insert(tree, 25, NULL);
    tree_insert(tree, 40, NULL);
    tree_insert(tree, 80, NULL);
    
    tree_debug(tree);
    return 0;
}

