/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>

#include <libyuc/container/experimental/avl_tree.def>

/*
* 嵌入平衡因子的Avl树
*/


#include <libyuc/container/experimental/avl_tree_AvlBsTree.cfg>
#include <libyuc/container/bs_tree.h>



typedef struct AvlEntry {
    union {
        struct {
            LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id left;
            LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id right;
        };
        AvlBsEntry bs_entry;
    };
} AvlEntry;
typedef struct AvlTree {
    union {
        AvlBsTree bs_tree;
        LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id root;
    };
} AvlTree;



void AvlTreeInit(AvlTree* tree);
LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id AvlTreeFind(AvlTree* tree, AvlBsTreeStackVector* stack, LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Key* key);
LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id AvlTreePut(AvlTree* tree, AvlBsTreeStackVector* stack, LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id put_entry_id);
bool AvlTreeDelete(AvlTree* tree, AvlBsTreeStackVector* stack, LIBYUC_CONTAINER_AVL_TREE_REFERENCER_Type_Id del_entry_id);
bool AvlTreeVerify(AvlTree* tree);


#include <libyuc/container/experimental/avl_tree.undef>