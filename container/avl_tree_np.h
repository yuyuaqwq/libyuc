

/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_AVLTREE_NP_H_
#define CUTILS_CONTAINER_AVLTREE_NP_H_

#include <CUtils/object.h>
#include <CUtils/container/bs_tree_np.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 无父结点嵌入平衡因子的AVL树
*/
typedef struct _AVLEntry {
    union {
        struct {
            struct _AVLEntry* left_balanceFactor;     // 平衡因子嵌入到左指针低2位
            struct _AVLEntry* right;
        };
        BSEntry bse;
    };
} AVLEntry;

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

AVLEntry* AVLEntryGetLeft(AVLEntry* entry);
AVLEntry* AVLEntryGetRight(AVLEntry* entry);
int AVLEntryGetBalanceFactor(AVLEntry* entry);

void AVLTreeInit(AVLTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
#define AVLTreeInitByField(tree, objName, entryFieldName, keyFieldName) AVLTreeInit((tree), GetFieldOffset(objName, entryFieldName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName), NULL)
void AVLEntryInit(AVLEntry* entry);
AVLEntry* AVLTreeFindEntryByKey(AVLTree* tree, void* key);
bool AVLTreeInsertEntryByKey(AVLTree* tree, AVLEntry* entry);
AVLEntry* AVLTreeDeleteEntryByKey(AVLTree* tree, void* key);


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_AVLTREE_H_


