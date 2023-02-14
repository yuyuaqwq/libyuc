/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_BS_TREE_NP_H_
#define CUTILS_CONTAINER_BS_TREE_NP_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _BsEntryNp {
    struct _BsEntryNp* left;
    struct _BsEntryNp* right;
} BsEntryNp;

typedef struct _BsTreeNp {
    BsEntryNp* root;
    int entryFieldOffset;
    int keyFieldOffset;
    int keyFieldSize;
    CmpFunc cmpFunc;        // 间接调用增加一定开销
} BsTreeNp;

void BsTreeNpInit(BsTreeNp* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
void BsEntryNpInit(BsEntryNp* entry);
#define BsTreeNpFindEntryByField(tree, retObj, key, objName, entryFieldName, keyFieldName) { \
retObj = NULL; \
BsEntryNp* cur = (BsEntryNp*)((tree)->root); \
while (cur) { \
    objName* tempObj = GetObjByField(cur, objName, entryFieldName); \
    if (tempObj->keyFieldName < (key)) { \
        cur = cur->right; \
    } else if (tempObj->keyFieldName > (key)) { \
        cur = cur->left; \
    } else { \
        retObj = tempObj; break; \
    } \
} \
}
bool BsTreeNpInsertEntryByKey(BsTreeNp* tree, BsEntryNp* entry);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BS_TREE_NP_H_