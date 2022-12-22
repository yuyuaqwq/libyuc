#ifndef UTILS_AVL_H_
#define UTILS_AVL_H_

#include "CUtils/container/obj.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _AVLEntry {
	struct _AVLEntry* parent;
	struct _AVLEntry* left;
	struct _AVLEntry* right;
	int height;
} AVLEntry;

typedef struct _AVLHead {
	AVLEntry* root;
	int entryOffset;
	bool smallByteOrder;
	int keyOffset;
	int keyByteCount;
	int objByteCount;
} AVLHead;

typedef bool (*TraversalCallback)(AVLEntry* entry, void* arg);

void AVLHeadInit(AVLHead* head, int objSize, int entryOffset, int keyOffset, int keySize);

void AVLEntryInit(AVLEntry* entry);

#define AVLFindKeyM(head, retObj, key, objName, entryFieldName, keyFieldName) { \
	retObj = NULL; \
	AVLEntry* cur = (obj)->root; \
	while (cur) { \
		typeName* tempObj = GetObjFromField(cur, objName, entryFieldName); \
		if (tempObj->keyFieldName < (key)) { \
			cur = cur->right; \
		} else if (tempObj->keyFieldName > (key)) { \
			cur = cur->left; \
		} else { \
			retObj = tempObj; break; \
		} \
	} \
}

AVLEntry* AVLFindKey(AVLHead* head, void* key);

bool AVLInsertEntry(AVLHead* head, AVLEntry* entry);

AVLEntry* AVLDeleteEntry(AVLHead* head, void* key);

size_t AVLEntrySize(AVLHead* head);

void AVLPreorder_Callback(AVLEntry* entry, TraversalCallback callback, void* arg);

#ifdef __cplusplus
}
#endif

#endif // UTILS_AVL_H_