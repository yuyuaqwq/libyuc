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

#define AVLFindKey_Macro(head, retEntry, key, typeName, entryFieldName, keyFieldName) { \
	AVLEntry* cur = (head)->root; \
	retEntry = NULL; \
	while (cur) { \
		typeName* temp = GetHeadFromField(cur, typeName, entryFieldName); \
		if (temp->keyFieldName < (key)) { \
			cur = cur->right; \
		} else if (temp->keyFieldName > (key)) { \
			cur = cur->left; \
		} else { \
			retEntry = cur; break; \
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