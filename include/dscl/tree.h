
typedef struct dscl_treenode dscl_treenode_t;
typedef struct dscl_tree dscl_tree_t;
dscl_tree_t* tree_create(int key, void* value);
void tree_insert(dscl_tree_t* tree, int key, void* value);
void tree_debug(dscl_tree_t* tree);

