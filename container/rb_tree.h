/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_RB_TREE_H_
#define CUTILS_CONTAINER_RB_TREE_H_

#include "CUtils/object.h"
#include "CUtils/container/bs_tree.h"

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
            struct _RBEntry* parent_color;      // 颜色嵌入到父节点指针的低1位
            struct _RBEntry* left;
            struct _RBEntry* right;
        };
        BSEntry bse;
    };
} RBEntry;

typedef struct _RBTree {
    union {
        struct {
            RBEntry* root;
            int entryFieldOffset;
            int keyFieldOffset;
            int keyFieldSize;
            CmpFunc cmpFunc;        // 间接调用增加一定开销
        };
        BSTree bst;
    };
} RBTree;

void RBTreeInit(RBTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
#define RBTreeInitByField(tree, objName, entryFieldName, keyFieldName) RBTreeInit((tree), GetFieldOffset(objName, entryFieldName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName), NULL)
void RBEntryInit(RBEntry* entry, RBColor color);
RBEntry* RBTreeFindEntryByKey(RBTree* tree, void* key);
void RBTreeInsertEntryBalance(RBTree* tree, RBEntry* entry);
bool RBTreeInsertEntryByKey(RBTree* tree, RBEntry* entry);
void RBTreeDeleteEntry(RBTree* tree, RBEntry* entry);
RBEntry* RBTreeDeleteEntryByKey(RBTree* tree, void* key);


inline RBColor RBEntryGetColor(RBEntry* entry);
inline void RBEntrySetColor(RBEntry* entry, RBColor color);
inline RBEntry* RBEntryGetParent(RBEntry* entry);
inline void RBEntrySetParent(RBEntry* entry, RBEntry* parent);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_RB_TREE_H_