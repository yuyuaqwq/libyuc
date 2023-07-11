

/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_AVL_TREE_NP_H_
#define LIBYUC_CONTAINER_AVL_TREE_NP_H_

#include <libyuc/object.h>
#include <libyuc/container/experimental/bs_tree_np.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 无父结点嵌入平衡因子的Avl树
*/
typedef struct _AvlEntryNp {
    union {
        struct {
            struct _AvlEntryNp* left_balanceFactor;     // 平衡因子嵌入到左指针低2位
            struct _AvlEntryNp* right;
        };
        BsEntryNp bse;
    };
} AvlEntryNp;

typedef struct _AvlTreeNp {
    union {
        struct {
            AvlEntryNp* root;
            int entryFieldOffset;
            int keyFieldOffset;
            int keyFieldSize;
            CmpFunc cmpFunc;        // 间接调用增加一定开销
        };
        BsTreeNp bst;
    };
} AvlTreeNp;

AvlEntryNp* AvlEntryNpGetLeft(AvlEntryNp* entry);
AvlEntryNp* AvlEntryNpGetRight(AvlEntryNp* entry);
int AvlEntryNpGetBalanceFactor(AvlEntryNp* entry);

void AvlTreeNpInit(AvlTreeNp* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
#define AvlTreeNpInitByField(tree, objName, entryFieldName, keyFieldName) AvlTreeInit((tree), ObjectGetFieldOffset(objName, entryFieldName), ObjectGetFieldOffset(objName, keyFieldName), ObjectGetFieldSize(objName, keyFieldName), NULL)
void AvlEntryNpInit(AvlEntryNp* entry);
void* AvlTreeNpFindEntryByKey(AvlTreeNp* tree, void* key);
bool AvlTreeNpInsertEntryByKey(AvlTreeNp* tree, AvlEntryNp* entry);
void* AvlTreeNpDeleteEntryByKey(AvlTreeNp* tree, void* key);


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_AVL_TREE_NP_H_