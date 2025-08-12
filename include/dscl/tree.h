#ifndef DSCL_TREE_H
#define DSCL_TREE_H

#define T_RED 1
#define T_BLACK 0

typedef struct dscl_treenode {
    int color;
    int key;
    void* value;

    struct dscl_treenode* parent;
    struct dscl_treenode* left;
    struct dscl_treenode* right;
} dscl_treenode_t;

typedef struct dscl_tree {
    dscl_treenode_t* root;
} dscl_tree_t;

dscl_tree_t* tree_create(int key, void* value);
void tree_insert(dscl_tree_t* tree, int key, void* value);
void tree_debug(dscl_tree_t* tree);

#endif //DSCL_TREE_H
