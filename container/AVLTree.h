#ifndef CUTILS_AVLTREE_H_
#define CUTILS_AVLTREE_H_

#include "CUtils/container/object.h"
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
	int height;
} AVLEntry;

typedef BSTree AVLTree;

#define AVLTreeInitM(tree, objName, entryFieldName, keyFieldName) AVLTreeInit((tree), sizeof(objName), GetFieldOffset(objName, entryFieldName), GetFieldOffset(objName, keyFieldName), GetFieldSize(objName, keyFieldName), NULL)

void AVLTreeInit(AVLTree* tree, int objSize, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);

void AVLEntryInit(AVLEntry* entry);

#define AVLFindEntryByKeyM(tree, retObj, key, objName, entryFieldName, keyFieldName) { \
	retObj = NULL; \
	AVLEntry* cur = (tree)->root; \
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

AVLEntry* AVLFindEntryByKey(AVLTree* tree, void* key);

bool AVLInsertEntry(AVLTree* tree, AVLEntry* entry);

AVLEntry* AVLDeleteEntry(AVLTree* tree, AVLEntry* entry);

AVLEntry* AVLDeleteEntryByKey(AVLTree* tree, void* key);

size_t AVLGetEntryCount(AVLTree* tree);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_AVLTREE_H_