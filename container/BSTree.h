#ifndef CUTILS_BSTREE_H_
#define CUTILS_BSTREE_H_

#include "CUtils/container/object.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _BSEntry {
	struct _BSEntry* parent;
	struct _BSEntry* left;
	struct _BSEntry* right;
} BSEntry;

typedef struct _BSTree {
	BSEntry* root;
	int entryFieldOffset;
	int keyFieldOffset;
	int keyFieldSize;
	CmpFunc cmpFunc;		// 间接调用增加一定开销
} BSTree;

void BSTreeInit(BSTree* tree, int entryFieldOffset, int keyFieldOffset, int keySize, CmpFunc cmpFunc);
void BSEntryInit(BSEntry* entry);
BSEntry* BSFindEntryByKey(BSTree* tree, void* key);
bool BSInsertEntry(BSTree* tree, BSEntry* entry);
BSEntry* BSDeleteEntry(BSTree* tree, BSEntry* entry);

BSEntry* BSFirst(BSTree* tree);
BSEntry* BSLast(BSTree* tree);
BSEntry* BSNext(BSEntry* entry);
BSEntry* BSPrev(BSEntry* entry);

typedef bool (*BSTraversalCallback)(BSEntry* entry, void* arg);
void BSPreorder_Callback(BSEntry* entry, BSTraversalCallback callback, void* arg);
void BSMiddleorder_Callback(BSEntry* entry, BSTraversalCallback callback, void* arg);
void BSPostorder_Callback(BSEntry* entry, BSTraversalCallback callback, void* arg);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_BSTREE_H_