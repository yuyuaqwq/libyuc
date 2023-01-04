#ifndef CUTILS_RBTREE_H_
#define CUTILS_RBTREE_H_

#include "CUtils/container/object.h"

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
	struct _RBEntry* parent;
	struct _RBEntry* left;
	struct _RBEntry* right;
	RBColor color;
} RBEntry;

typedef struct _RBTree {
	RBEntry* root;
	// bool smallByteOrder;
	int objSize;
	int entryFieldOffset;
	int keyFieldOffset;
	int keyFieldSize;
} RBTree;


void RBTreeInit(RBTree* tree, int objSize, int entryFieldOffset, int keyFieldOffset, int keySize);

#define RBTreeInitM(tree, objName, entryFieldName, keyFieldName) RBTreeInit((tree), sizeof(objName), GetFieldOffset(objName, entryFieldName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName))

void RBEntryInit(RBEntry* entry, RBColor color);

bool RBInsertEntry(RBTree* tree, RBEntry* entry);
RBEntry* RBDeleteEntry(RBTree* tree, RBEntry* entry);
RBEntry* RBDeleteEntryByKey(RBTree* tree, void* key);
size_t RBGetEntryCount(RBTree* head);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_RBTREE_H_