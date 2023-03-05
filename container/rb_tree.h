/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_RB_TREE_H_
#define CUTILS_CONTAINER_RB_TREE_H_

#include <CUtils/object.h>
#include <CUtils/container/experimental/bs_tree.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
* 以234树为原型的红黑树实现
*/
typedef enum _RbColor RbColor;

typedef struct _RbEntry {
    union {
        struct {
            struct _RbEntry* parent_color;      // 颜色嵌入到父节点指针的低1位
            struct _RbEntry* left;
            struct _RbEntry* right;
        };
        BsEntry bse;
    };
} RbEntry;

typedef struct _RbTree {
    union {
        struct {
            RbEntry* root;
            int entry_field_offset;
            int key_field_offset;
            int key_field_size;
            CmpFunc cmp_func;        // 间接调用增加一定开销
        };
        BsTree bst;
    };
} RbTree;

typedef struct _RbTreeIterator {
    RbTree* rb_tree;
    RbEntry* cur_entry;
} RbTreeIterator;

RbColor RbEntryGetColor(RbEntry* entry);
void RbEntrySetColor(RbEntry* entry, RbColor color);
RbEntry* RbEntryGetParent(RbEntry* entry);
void RbEntrySetParent(RbEntry* entry, RbEntry* parent);

void RbTreeInsertEntryFixup(RbTree* tree, RbEntry* entry);
bool RbTreeInsertEntryByKey(RbTree* tree, RbEntry* entry);

void RbTreeInit(RbTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
#define RbTreeInitByField(tree, objName, entryFieldName, keyFieldName) RbTreeInit((tree), ObjectGetFieldOffset(objName, entryFieldName), ObjectGetFieldOffset(objName, keyFieldName), ObjectGetFieldSize(objName, keyFieldName), NULL)
void RbEntryInit(RbEntry* entry, RbColor color);
void* RbTreeFindEntryByKey(RbTree* tree, void* key);
void RbTreeDeleteEntryFixup(RbTree* tree, RbEntry* entry, RbEntry* parent, RbEntry* left, RbEntry* right, RbColor color, bool isLeft);
void RbTreeDeleteEntry(RbTree* tree, RbEntry* entry);
void* RbTreeDeleteEntryByKey(RbTree* tree, void* key);

void* RbTreeFirst(RbTree* tree, RbTreeIterator* iterator);
void* RbTreeLast(RbTree* tree, RbTreeIterator* iterator);
void* RbTreeNext(RbTreeIterator* iterator);
void* RbTreePrev(RbTreeIterator* iterator);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_RB_TREE_H_