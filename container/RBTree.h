#ifndef CUTILS_RBTREE_H_
#define CUTILS_RBTREE_H_

#include "CUtils/container/object.h"
#include "CUtils/container/BSTree.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* 以234树为原型的红黑树实现
*/
typedef enum _RBColor {
	kBlack,
	kRed,
} RBColor;
typedef struct _RBEntry {
	union {
		struct {
			struct _RBEntry* parent;
			struct _RBEntry* left;
			struct _RBEntry* right;
		};
		BSEntry bs;
	};

	RBColor color;
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

#ifdef __cplusplus
}
#endif

#endif // CUTILS_RBTREE_H_