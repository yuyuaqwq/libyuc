/*
* @yuyuaqwq - 鱼鱼
* emali:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_RBTREE_H_
#define CUTILS_CONTAINER_RBTREE_H_

#include "CUtils/object.h"
#include "CUtils/container/BSTree.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* 以234树为原型的红黑树实现
*/
typedef enum _RBColor RBColor;
typedef struct _RBEntry {
    union {
        struct {
            struct _RBEntry* parent_color;
            struct _RBEntry* left;
            struct _RBEntry* right;
        };
        BSEntry bs;
    };
} RBEntry;

typedef BSTree RBTree;

void RBTreeInit(RBTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
#define RBTreeInitM(tree, objName, entryFieldName, keyFieldName) RBTreeInit((tree), GetFieldOffset(objName, entryFieldName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName), NULL)
void RBEntryInit(RBEntry* entry, RBColor color);
RBEntry* RBFindEntryByKey(RBTree* tree, void* key);
bool RBInsertEntry(RBTree* tree, RBEntry* entry);
RBEntry* RBDeleteEntry(RBTree* tree, RBEntry* entry);
RBEntry* RBDeleteEntryByKey(RBTree* tree, void* key);
size_t RBGetEntryCount(RBTree* head);

void PrintRB(RBTree* head, RBEntry* entry, int Level);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_RBTREE_H_