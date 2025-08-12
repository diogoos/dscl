#include "dscl/tree.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int node_insert_helper(dscl_treenode_t* node, dscl_treenode_t* new);
void balance_tree(dscl_treenode_t* node);
static void tree_rotate_left(dscl_tree_t* tree, dscl_treenode_t* node);
static void tree_rotate_right(dscl_tree_t* tree, dscl_treenode_t* node);

dscl_tree_t* tree_create(int key, void* value) {
    dscl_treenode_t* root = malloc(sizeof(dscl_treenode_t));
    if (root == NULL) return NULL;
    root->color = T_BLACK;
    root->key = key;
    root->value = value;
    root->left = NULL;
    root->right = NULL;
    root->parent = NULL;

    dscl_tree_t* tree = malloc(sizeof(dscl_tree_t));
    if (tree == NULL) {
        free(root);
        return NULL;
    }
    tree->root = root;
    return tree;
}

void tree_insert(dscl_tree_t* tree, int key, void* value) {
    dscl_treenode_t* node = malloc(sizeof(dscl_treenode_t));
    node->color = T_RED; // new nodes are initially red
    node->key = key;
    node->value = value;
    node->left = node->right = node->parent = NULL;

    // initially, naively insert at best location
    int insert = node_insert_helper(tree->root, node);
    if (insert == 0) return; // if this was just a value update, we're done

    // rebalance the tree
    dscl_treenode_t* parent;
    while ((parent = node->parent) != NULL) {
        dscl_treenode_t* grandparent = parent->parent;

        // trivial cases -- already balanced, or simple recolor
        if (parent->color == T_BLACK) return;
        if (!grandparent) {
            parent->color = T_BLACK;
            return;
        }

        // find uncle
        dscl_treenode_t* uncle;
        if (parent == grandparent->left) uncle = grandparent->right;
                                    else uncle = grandparent->left;

        // Case 1: recolor upwards
        if (uncle && uncle->color == T_RED) {
            parent->color = T_BLACK;
            uncle->color = T_BLACK;
            grandparent->color = T_RED;
            node = grandparent; // now recolor upwards again
        } else {
            /*
            * Check for Case 2: P alignment opposite to GP alignment
            *
            *      G                 G
            *     / \               / \
            *     P       or           P
            *    / \                  / \
            *       N                N
            *  rotate left       rotate right
            */
            dscl_treenode_t* aligned_child;
            if (parent == grandparent->left) aligned_child = parent->left;
                                        else aligned_child = parent->right;
            if (node != aligned_child) {
                // We are in Case 2 - perform an anticlockwise rotation
                if (parent->left == node) tree_rotate_right(tree, parent);
                                     else tree_rotate_left(tree, parent);

                node = parent;
                parent = node->parent;                
            }

            /*
            * If the tree was already aligned, or we have just fixed the
            * subtree in case 2, we need a case 3 rotation:
            *
            *      G                    P
            *     / \                  / \
            *     P      becomes      N    G
            *    / \                 / \  / \
            *   N                   
            */
            parent->color = T_BLACK;
            grandparent->color = T_RED;
            if (parent == grandparent->left) tree_rotate_right(tree, grandparent);
                                        else tree_rotate_left(tree, grandparent);
        }
    }

    tree->root->color = T_BLACK;
}

static void tree_rotate_left(dscl_tree_t* tree, dscl_treenode_t* node) {
    dscl_treenode_t* sub_parent = node->parent;
    dscl_treenode_t* new_root = node->right;
    if (new_root == NULL) return;
    dscl_treenode_t* new_child = new_root->left;

    node->right = new_child;
    if (new_child) new_child->parent = node;

    new_root->left = node;
    new_root->parent = sub_parent;
    node->parent = new_root;
    if (sub_parent) {
        if (node == sub_parent->right) sub_parent->right = new_root;
                                 else  sub_parent->left = new_root;
    } else {
        tree->root = new_root;
    }
}

static void tree_rotate_right(dscl_tree_t* tree, dscl_treenode_t* node) {
    dscl_treenode_t* sub_parent = node->parent;
    dscl_treenode_t* new_root = node->left;
    if (new_root == NULL) return;
    dscl_treenode_t* new_child = new_root->right;

    node->left = new_child;
    if (new_child) new_child->parent = node;

    new_root->right = node;
    new_root->parent = sub_parent;
    node->parent = new_root;
    if (sub_parent) {
        if (node == sub_parent->right) sub_parent->right = new_root;
                                 else  sub_parent->left = new_root;
    } else {
        tree->root = new_root;
    }
}

/**
 * Naively inserts a node into the tree, as per BST
 * Returns 0 if it is just a value update, 1 if a new node is
 * inserted.
 */
int node_insert_helper(dscl_treenode_t* node, dscl_treenode_t* new) {
    if (node->key == new->key) { // if a key already exists, override it (no duplicates)
        node->value = new->value;
        return 0;
    }
    else if (new->key > node->key) { // insert right
        if (node->right == NULL) {
            node->right = new;
            new->parent = node;
            return 1;
        } else {
            return node_insert_helper(node->right, new);
        }
    } else { // insert left
        if (node->left == NULL) {
            node->left = new;
            new->parent = node;
            return 1;
        } else {
            return node_insert_helper(node->left, new);
        }
    }
}

void tree_debug_helper(dscl_treenode_t* node, int depth) {
    for (int i = 0; i < depth; i++) printf("   ");
    for (int i = 0; i < depth; i++) printf(">");
    printf("%d(%d)\n", node->key, node->color);
    if(node->left != NULL) tree_debug_helper(node->left, depth+1);
    if(node->right != NULL) tree_debug_helper(node->right, depth+1);
}

void tree_debug(dscl_tree_t* tree) {
    tree_debug_helper(tree->root, 0);
}

