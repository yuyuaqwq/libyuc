#ifndef CUTILS_BSTREE_H_
#define CUTILS_BSTREE_H_

#include "CUtils/container/object.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _BSEntry {
	struct _AVLEntry* parent;
	struct _AVLEntry* left;
	struct _AVLEntry* right;
} BSEntry;

typedef struct _BSTree {
	BSEntry* root;
	// bool smallByteOrder;
	int objSize;
	int entryFieldOffset;
	int keyFieldOffset;
	int keyFieldSize;
} BSTree;




#ifdef __cplusplus
}
#endif

#endif // CUTILS_BSTREE_H_