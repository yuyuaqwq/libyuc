/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/
#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libyuc/container/rb_tree.def>

#include <libyuc/container/rb_tree_RbBsTree.cfg>
#include <libyuc/container/bs_tree.h>

#define kRbBlack 0
#define kRbRed 1
typedef int RbColor;

//typedef enum {
//    kRbBlack,
//    kRbRed,
//} RbColor;

typedef struct RbEntry {
    union {
        struct {
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id left;
            LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id right;
        };
        RbBsEntry bs_entry;
    };
} RbEntry;
typedef struct RbTree {
    union {
        RbBsTree bs_tree;
        LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id root;
    };
} RbTree;
typedef struct RbTreeIterator {
    LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id cur_id;
    RbBsTreeStackVector stack;
} RbTreeIterator;
   


void RbTreeInit(RbTree* tree);
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreeFind(RbTree* tree, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Key* key);
bool RbTreeInsert(RbTree* tree, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id insert_entry_id);
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreePut(RbTree* tree, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id put_entry_id);
bool RbTreeDelete(RbTree* tree, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Key* key);
bool RbTreeDeleteByIterator(RbTree* tree, RbTreeIterator* iterator);
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Offset RbTreeGetCount(RbTree* tree);
   
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreeIteratorLocate(RbTree* tree, RbTreeIterator* iterator, LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Key* key, LIBYUC_CONTAINER_RB_TREE_COMPARER_Type_Diff* cmp_diff);
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreeIteratorFirst(RbTree* tree, RbTreeIterator* iterator);
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreeIteratorLast(RbTree* tree, RbTreeIterator* iterator);
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreeIteratorNext(RbTree* tree, RbTreeIterator* iterator);
LIBYUC_CONTAINER_RB_TREE_REFERENCER_Type_Id RbTreeIteratorPrev(RbTree* tree, RbTreeIterator* iterator);
void RbTreeIteratorCopy(RbTreeIterator* dst_iterator, const RbTreeIterator* src_iterator);
   
bool RbTreeVerify(RbTree* tree);





#include <libyuc/container/rb_tree.undef>


#ifdef __cplusplus
}
#endif
