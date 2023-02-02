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

typedef struct _BSEntryNp {
    struct _BSEntryNp* left;
    struct _BSEntryNp* right;
} BSEntryNp;

typedef struct _BSTreeNp {
    BSEntryNp* root;
    int entryFieldOffset;
    int keyFieldOffset;
    int keyFieldSize;
    CmpFunc cmpFunc;        // 间接调用增加一定开销
} BSTreeNp;

void BSTreeNpInit(BSTreeNp* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
void BSEntryNpInit(BSEntryNp* entry);
BSEntryNp* BSTreeNpFindEntryByKey(BSTreeNp* tree, void* key);
#define BSTreeNpFindEntryByField(tree, retObj, key, objName, entryFieldName, keyFieldName) { \
retObj = NULL; \
BSEntryNp* cur = (BSEntryNp*)((tree)->root); \
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
bool BSTreeNpInsertEntryByKey(BSTreeNp* tree, BSEntryNp* entry);
BSEntryNp* BSTreeNpDeleteEntry(BSTreeNp* tree, BSEntryNp* entry, BSEntryNp* entryParent);
size_t BSTreeNpGetEntryCount(BSTreeNp* tree);


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BS_TREE_H_