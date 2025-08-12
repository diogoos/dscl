#include "dscl/tree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define T_RED 0
#define T_BLACK 1

void node_insert_helper(dscl_treenode_t* node, dscl_treenode_t* new);

typedef struct dscl_treenode {
    int color;
    int key;
    void* value;

    dscl_treenode_t* parent;
    dscl_treenode_t* left;
    dscl_treenode_t* right;
} dscl_treenode_t;

typedef struct dscl_tree {
    dscl_treenode_t* root;
} tree_t;

tree_t* tree_create(int key, void* value) {
    dscl_treenode_t* root = malloc(sizeof(dscl_treenode_t));
    if (root == NULL) return NULL;
    root->color = T_RED;
    root->key = key;
    root->value = value;
    root->left = NULL;
    root->right = NULL;
    root->parent = NULL;

    tree_t* tree = malloc(sizeof(dscl_treenode_t));
    if (tree == NULL) {
        free(root);
        return NULL;
    }
    tree->root = root;
    return tree;
}

void tree_insert(tree_t* tree, int key, void* value) {
    dscl_treenode_t* new_node = malloc(sizeof(dscl_treenode_t));
    new_node->color = T_RED; // new nodes are initially red
    new_node->key = key;
    new_node->value = value;

    // initially, naively insert at best location
    node_insert_helper(tree->root, new_node);
}

void node_insert_helper(dscl_treenode_t* node, dscl_treenode_t* new) {
    if (node->key == new->key) { // if a key already exists, override it (no duplicates)
        node->value = new->value;
    }
    else if (node->key < new->key) { // insert left
        if (node->left == NULL) node->left = new;
        else node_insert_helper(node->left, new);
    }
    else { // insert right
        if (node->right == NULL) node->right = new;
        else node_insert_helper(node->right, new);
    }
}

void tree_debug_helper(dscl_treenode_t* node, int depth) {
    for (int i = 0; i < depth; i++) printf("   ");
    for (int i = 0; i < depth; i++) printf(">");
    printf("%d\n", node->key);
    if(node->left != NULL) tree_debug_helper(node->left, depth+1);
    if(node->right != NULL) tree_debug_helper(node->right, depth+1);
}

void tree_debug(tree_t* tree) {
    tree_debug_helper(tree->root, 0);
}

