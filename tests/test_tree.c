#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dscl/tree.h"

int main(void) {
    dscl_tree_t* tree = tree_create(30, NULL);
    if (!tree) {
        fprintf(stderr, "Failed to create tree\n");
        return 1;
    }

    // Insert a variety of keys
    int keys[] = {10, 20, 40, 50, 25, 5, 1, 15, 35, 45, 60, 70, 65, 55, 80};
    size_t n_keys = sizeof(keys) / sizeof(keys[0]);
    for (size_t i = 0; i < n_keys; i++) {
        tree_insert(tree, keys[i], NULL);
    }

    dscl_treenode_t* node20 = tree->root;
    assert(node20->color == T_BLACK);
    assert(node20->key == 20);
    
    dscl_treenode_t* node05 = node20->left;
    assert(node05->color == T_BLACK);
    assert(node05->key == 05);
    dscl_treenode_t* node50 = node20->right;
    assert(node50->color == T_BLACK);
    assert(node50->key == 50);

    dscl_treenode_t* node01 = node05->left;
    assert(node01->color == T_BLACK);
    assert(node01->key == 01);
    assert(node01->left == NULL);
    assert(node01->right == NULL);
    dscl_treenode_t* node10 = node05->right;
    assert(node10->color == T_BLACK);
    assert(node10->key == 10);
    assert(node10->left == NULL);
    dscl_treenode_t* node40 = node50->left;
    assert(node40->color == T_RED);
    assert(node40->key == 40);
    dscl_treenode_t* node65 = node50->right;
    assert(node65->color == T_RED);
    assert(node65->key == 65);

    dscl_treenode_t* node15 = node10->right;
    assert(node15->color == T_RED);
    assert(node15->key == 15);
    assert(node15->left == NULL);
    assert(node15->right == NULL);
    dscl_treenode_t* node30 = node40->left;
    assert(node30->color == T_BLACK);
    assert(node30->key == 30);
    dscl_treenode_t* node45 = node40->right;
    assert(node45->color == T_BLACK);
    assert(node45->key == 45);
    assert(node45->left == NULL);
    assert(node45->right == NULL);
    dscl_treenode_t* node60 = node65->left;
    assert(node60->color == T_BLACK);
    assert(node60->key == 60);
    assert(node60->right == NULL);
    dscl_treenode_t* node70 = node65->right;
    assert(node70->color == T_BLACK);
    assert(node70->key == 70);
    assert(node70->left == NULL);

    dscl_treenode_t* node25 = node30->left;
    assert(node25->color == T_RED);
    assert(node25->key == 25);
    assert(node25->left == NULL);
    assert(node25->right == NULL);
    dscl_treenode_t* node35 = node30->right;
    assert(node35->color == T_RED);
    assert(node35->key == 35);
    assert(node35->left == NULL);
    assert(node35->right == NULL);
    dscl_treenode_t* node55 = node60->left;
    assert(node55->color == T_RED);
    assert(node55->key == 55);
    assert(node55->left == NULL);
    assert(node55->right == NULL);
    dscl_treenode_t* node80 = node70->right;
    assert(node80->color == T_RED);
    assert(node80->key == 80);
    assert(node80->left == NULL);
    assert(node80->right == NULL);
    
    printf("Regression test passed!\n");
    return 0;
}

