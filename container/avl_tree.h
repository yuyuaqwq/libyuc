/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_AVL_TREE_H_
#define CUTILS_CONTAINER_AVL_TREE_H_

#include <CUtils/object.h>
#include <CUtils/container/bs_tree.h>

#ifdef __cplusplus
extern "C" {
#endif

// #define CUTILS_CONTAINER_AVL_TREE_STORAGE_HEIGHT_H_       // 是否存储高度，存储平衡因子可以节省空间

#if !defined(CUTILS_CONTAINER_AVL_TREE_STORAGE_HEIGHT_H_)
/*
* 嵌入平衡因子的Avl树
*/
typedef struct _AvlEntry {
    union {
        struct {
            struct _AvlEntry* parent_balanceFactor;     // 平衡因子嵌入到父指针低2位
            struct _AvlEntry* left;
            struct _AvlEntry* right;
        };
        BsEntry bse;
    };
} AvlEntry;
#else
/*
* 存储高度的Avl树
*/
typedef struct _AvlEntry {
    union {
        struct {
            struct _AvlEntry* parent;
            struct _AvlEntry* left;
            struct _AvlEntry* right;
        };
        BSEntry bse;
    };
    int height;        // 这里如果换成平衡因子(2位即可)也可以嵌入到指针(低2位可以不用)中，高度更易理解与实现
} AvlEntry;
#endif // CUTILS_CONTAINER_Avl_TREE_STORAGE_HEIGHT_H_

typedef struct _AvlTree {
    union {
        struct {
            AvlEntry* root;
            int entryFieldOffset;
            int keyFieldOffset;
            int keyFieldSize;
            CmpFunc cmpFunc;        // 间接调用增加一定开销
        };
        BsTree bst;
    };
} AvlTree;

AvlEntry* AvlEntryGetParent(AvlEntry* entry);
AvlEntry* AvlEntryGetLeft(AvlEntry* entry);
AvlEntry* AvlEntryGetRight(AvlEntry* entry);
int AvlEntryGetBalanceFactor(AvlEntry* entry);


void AvlTreeInsertEntryFixup(AvlTree* tree, AvlEntry* entry);
void AvlTreeDeleteEntryFixup(AvlTree* tree, AvlEntry* parent, bool isLeft);


void AvlTreeInit(AvlTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
#define AvlTreeInitByField(tree, objName, entryFieldName, keyFieldName) AvlTreeInit((tree), GetFieldOffset(objName, entryFieldName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName), NULL)
void AvlEntryInit(AvlEntry* entry);
void* AvlTreeFindEntryByKey(AvlTree* tree, void* key);
bool AvlTreeInsertEntryByKey(AvlTree* tree, AvlEntry* entry);
void AvlTreeDeleteEntry(AvlTree* tree, AvlEntry* entry);
void* AvlTreeDeleteEntryByKey(AvlTree* tree, void* key);


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_AVL_TREE_H_