/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libyuc/container/bs_tree.def>


typedef struct BsEntry {
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id left;
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id right;
} BsEntry;
typedef struct BsTree {
    LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id root;
} BsTree;
   
#include <libyuc/container/bs_tree_BsTreeStackVector.cfg>
#include <libyuc/container/vector.h>


void BsTreeInit(BsTree* tree);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeFind(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_Type_Key* key);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreePut(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id entry_id);
bool BsTreeInsert(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id entry_id);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeDelete(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id entry_id);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Offset BsTreeGetCount(BsTree* tree);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorLocate(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_Type_Key* key, int8_t* cmp_status);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorFirst(BsTree* tree, BsTreeStackVector* stack);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorLast(BsTree* tree, BsTreeStackVector* stack);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorNext(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id cur_id);
LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id BsTreeIteratorPrev(BsTree* tree, BsTreeStackVector* stack, LIBYUC_CONTAINER_BS_TREE_REFERENCER_Type_Id cur_id);

#include <libyuc/container/bs_tree.undef>

#ifdef __cplusplus
}
#endif
