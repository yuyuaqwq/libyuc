/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_AVLTREE_H_
#define CUTILS_CONTAINER_AVLTREE_H_

#include "CUtils/object.h"
#include "CUtils/container/BSTree.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _AVLEntry {
    union {
        struct {
            struct _AVLEntry* parent;
            struct _AVLEntry* left;
            struct _AVLEntry* right; 
        };
        BSEntry bs;
    };
    int height;        // 这里如果换成平衡因子(2位即可)也可以嵌入到指针(低2位可以不用)中，但是高度更易理解
} AVLEntry;

typedef BSTree AVLTree;

void AVLTreeInit(AVLTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
#define AVLTreeInitByField(tree, objName, entryFieldName, keyFieldName) AVLTreeInit((tree), GetFieldOffset(objName, entryFieldName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName), NULL)
void AVLEntryInit(AVLEntry* entry);
AVLEntry* AVLFindEntryByKey(AVLTree* tree, void* key);
bool AVLInsertEntry(AVLTree* tree, AVLEntry* entry);
AVLEntry* AVLDeleteEntry(AVLTree* tree, AVLEntry* entry);
AVLEntry* AVLDeleteEntryByKey(AVLTree* tree, void* key);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_AVLTREE_H_