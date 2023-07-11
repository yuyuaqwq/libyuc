/*
* @yuyuaqwq - ����
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/libyuc
* �뱣��������
*/

#ifndef LIBYUC_CONTAINER_BS_TREE_NP_H_
#define LIBYUC_CONTAINER_BS_TREE_NP_H_

#include <libyuc/object.h>

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
    CmpFunc cmpFunc;        // ��ӵ�������һ������
} BsTreeNp;

void BsTreeNpInit(BsTreeNp* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
void BsEntryNpInit(BsEntryNp* entry);
#define BsTreeNpFindEntryByField(tree, retObj, key, objName, entryFieldName, keyFieldName) { \
retObj = NULL; \
BsEntryNp* cur = (BsEntryNp*)((tree)->root); \
while (cur) { \
    objName* tempObj = ObjectGetFromField(cur, objName, entryFieldName); \
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

#endif // LIBYUC_CONTAINER_BS_TREE_NP_H_