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



#ifdef __cplusplus
}
#endif

#endif // CUTILS_RBTREE_H_