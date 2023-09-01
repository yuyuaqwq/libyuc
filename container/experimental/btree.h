/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_B_TREE_H_
#define LIBYUC_CONTAINER_B_TREE_H_

#include <libyuc/basic.h>
#include <libyuc/algorithm/binary_search.h>
#include <libyuc/container/list.h>
#include <libyuc/container/vector.h>
#include <libyuc/container/static_list.h>
#include <libyuc/container/rb_tree.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 为内存设计的b-tree
* 为支持变长kv的设计，不实现m阶等概念
*/


typedef int64_t key_type;
typedef key_type element_type;

typedef struct _BTreeEntry* entry_id;

#define LIBYUC_CONTAINER_B_TREE_M 65

typedef enum {
    kBTreeIteratorNe,
    kBTreeIteratorEq,
    kBTreeIteratorDown,
    kBTreeIteratorEnd,
} BTreeIteratorStatus;

LIBYUC_CONTAINER_VECTOR_DECLARATION(
    BIteratorStack,
    LIBYUC_CONTAINER_VECTOR_MODE_STATIC,
    uint32_t,
    entry_id,
    entry_id,
    sizeof(entry_id) * 8
)

LIBYUC_ALGORITHM_ARRAY_DEFINE(
    BTreeElement, 
    uint32_t, 
    element_type*, 
    element_type, 
    key_type, 
    LIBYUC_ALGORITHM_ARRAY_ACCESSOR_DEFAULT,
    LIBYUC_BASIC_COMPARER_DEFALUT, 
    LIBYUC_BASIC_REFERENCER_DEFALUT
)

typedef struct _BTree {
    entry_id root;
} BTree;

typedef struct _BTreeEntry {
    uint32_t count;
    element_type element[LIBYUC_CONTAINER_B_TREE_M-1];
    entry_id child[LIBYUC_CONTAINER_B_TREE_M];
} BTreeEntry;




typedef struct _BTreeIterator {
    BIteratorStackVector stack;
} BTreeIterator;

void BTreeEntryInit(entry_id entry) {
    entry->count = 0;
}

void BTreeInit(BTree* tree) {
    tree->root = ObjectCreate(BTreeEntry);
    BTreeEntryInit(tree->root);
}

void BTreePut(BTree* tree, element_type* element) {

}

bool BTreeFind(BTree* tree, key_type* key) {

}


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_B_TREE_H_