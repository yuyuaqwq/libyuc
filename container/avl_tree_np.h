

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
typedef struct _AVLEntryNp {
    union {
        struct {
            struct _AVLEntryNp* left_balanceFactor;     // 平衡因子嵌入到左指针低2位
            struct _AVLEntryNp* right;
        };
        BSEntryNp bse;
    };
} AVLEntryNp;

typedef struct _AVLTreeNp {
    union {
        struct {
            AVLEntryNp* root;
            int entryFieldOffset;
            int keyFieldOffset;
            int keyFieldSize;
            CmpFunc cmpFunc;        // 间接调用增加一定开销
        };
        BSTreeNp bst;
    };
} AVLTreeNp;

AVLEntryNp* AVLEntryNpGetLeft(AVLEntryNp* entry);
AVLEntryNp* AVLEntryNpGetRight(AVLEntryNp* entry);
int AVLEntryNpGetBalanceFactor(AVLEntryNp* entry);

void AVLTreeNpInit(AVLTreeNp* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
#define AVLTreeNpInitByField(tree, objName, entryFieldName, keyFieldName) AVLTreeInit((tree), GetFieldOffset(objName, entryFieldName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName), NULL)
void AVLEntryNpInit(AVLEntryNp* entry);
AVLEntryNp* AVLTreeNpFindEntryByKey(AVLTreeNp* tree, void* key);
bool AVLTreeNpInsertEntryByKey(AVLTreeNp* tree, AVLEntryNp* entry);
AVLEntryNp* AVLTreeNpDeleteEntryByKey(AVLTreeNp* tree, void* key);


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_AVLTREE_H_


