/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_AVLTREE_H_
#define CUTILS_CONTAINER_AVLTREE_H_

#include "CUtils/object.h"
#include "CUtils/container/bs_tree.h"

#ifdef __cplusplus
extern "C" {
#endif

// #define CUTILS_CONTAINER_AVL_TREE_STORAGE_HEIGHT_H_       // 是否存储高度，存储平衡因子可以节省空间

#ifndef CUTILS_CONTAINER_AVL_TREE_STORAGE_HEIGHT_H_
/*
* 嵌入平衡因子的AVL树
*/
typedef struct _AVLEntry {
    union {
        struct {
            struct _AVLEntry* parent_balanceFactor;     // 平衡因子嵌入到父指针低2位
            struct _AVLEntry* left;
            struct _AVLEntry* right;
        };
        BSEntry bse;
    };
} AVLEntry;
#else
/*
* 存储高度的AVL树
*/
typedef struct _AVLEntry {
    union {
        struct {
            struct _AVLEntry* parent;
            struct _AVLEntry* left;
            struct _AVLEntry* right;
        };
        BSEntry bse;
    };
    int height;        // 这里如果换成平衡因子(2位即可)也可以嵌入到指针(低2位可以不用)中，高度更易理解与实现
} AVLEntry;
#endif // CUTILS_CONTAINER_AVL_TREE_STORAGE_HEIGHT_H_

typedef struct _AVLTree {
    union {
        struct {
            AVLEntry* root;
            int entryFieldOffset;
            int keyFieldOffset;
            int keyFieldSize;
            CmpFunc cmpFunc;        // 间接调用增加一定开销
        };
        BSTree bst;
    };
} AVLTree;

void AVLTreeInit(AVLTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
#define AVLTreeInitByField(tree, objName, entryFieldName, keyFieldName) AVLTreeInit((tree), GetFieldOffset(objName, entryFieldName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName), NULL)
void AVLEntryInit(AVLEntry* entry);
AVLEntry* AVLTreeFindEntryByKey(AVLTree* tree, void* key);
void AVLTreeInsertEntryBalance(AVLTree* tree, AVLEntry* entry);
bool AVLTreeInsertEntryByKey(AVLTree* tree, AVLEntry* entry);
void AVLTreeDeleteEntry(AVLTree* tree, AVLEntry* entry);
AVLEntry* AVLTreeDeleteEntryByKey(AVLTree* tree, void* key);

AVLEntry* AVLEntryGetParent(AVLEntry* entry);
int AVLEntryGetBalanceFactor(AVLEntry* entry);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_AVLTREE_H_