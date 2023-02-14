/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_BS_TREE_H_
#define CUTILS_CONTAINER_BS_TREE_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _BsEntry {
    struct _BsEntry* parent;
    struct _BsEntry* left;
    struct _BsEntry* right;
} BsEntry;


typedef struct _BsTree {
    BsEntry* root;
    int entryFieldOffset;
    int keyFieldOffset;
    int keyFieldSize;
    CmpFunc cmpFunc;        // 间接调用增加一定开销
} BsTree;



void BsTreeInit(BsTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
void BsEntryInit(BsEntry* entry);
void* BsTreeFindEntryByKey(BsTree* tree, void* key);
#define BsTreeFindEntryByField(tree, retObj, key, objName, entryFieldName, keyFieldName) { \
    retObj = NULL; \
    BsEntry* cur = (BsEntry*)((tree)->root); \
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
bool BsTreeInsertEntryByKey(BsTree* tree, BsEntry* entry);
void* BsTreeDeleteEntry(BsTree* tree, BsEntry* entry);
size_t BsTreeGetEntryCount(BsTree* tree);

BsEntry* BsTreeFirst(BsTree* tree);
BsEntry* BsTreeLast(BsTree* tree);
BsEntry* BsTreeNext(BsEntry* entry);
BsEntry* BsTreePrev(BsEntry* entry);


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_BS_TREE_H_