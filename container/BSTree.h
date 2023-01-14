/*
* @yuyuaqwq - 鱼鱼
* emali:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_BSTREE_H_
#define CUTILS_CONTAINER_BSTREE_H_

#include "CUtils/object.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _BSEntry {
    struct _BSEntry* parent;
    struct _BSEntry* left;
    struct _BSEntry* right;
} BSEntry;

typedef struct _BSTree {
    BSEntry* root;
    int entryFieldOffset;
    int keyFieldOffset;
    int keyFieldSize;
    CmpFunc cmpFunc;        // 间接调用增加一定开销
} BSTree;

void BSTreeInit(BSTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
void BSEntryInit(BSEntry* entry);
BSEntry* BSFindEntryByKey(BSTree* tree, void* key);
#define BSFindEntryByField(tree, retObj, key, objName, entryFieldName, keyFieldName) { \
    retObj = NULL; \
    BSEntry* cur = (BSEntry*)((tree)->root); \
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
bool BSInsertEntry(BSTree* tree, BSEntry* entry);
BSEntry* BSDeleteEntry(BSTree* tree, BSEntry* entry);
size_t BSGetEntryCount(BSTree* tree);

BSEntry* BSFirst(BSTree* tree);
BSEntry* BSLast(BSTree* tree);
BSEntry* BSNext(BSEntry* entry);
BSEntry* BSPrev(BSEntry* entry);

typedef bool (*BSTraversalCallback)(BSEntry* entry, void* arg);
void BSPreorder_Callback(BSEntry* entry, BSTraversalCallback callback, void* arg);
void BSMiddleorder_Callback(BSEntry* entry, BSTraversalCallback callback, void* arg);
void BSPostorder_Callback(BSEntry* entry, BSTraversalCallback callback, void* arg);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BSTREE_H_