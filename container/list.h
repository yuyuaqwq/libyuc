#ifndef UTILS_LIST_H_
#define UTILS_LIST_H_

#include "CUtils/container/obj.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _ListEntry {
	struct _ListEntry* prev;
	struct _ListEntry* next;
} ListEntry;

void ListHeadInit(ListEntry* head);

bool ListHeadEmpty(ListEntry* entry);

void ListInsertHead(ListEntry* head, ListEntry* entry);

void ListInsertTail(ListEntry* head, ListEntry* entry);

ListEntry* ListRemoveEntry(ListEntry* entry, bool empty);

ListEntry* ListRemoveHead(ListEntry* head);

ListEntry* ListRemoveTail(ListEntry* head);

size_t ListEntryCount(ListEntry* head);

bool ListIteration(ListEntry* head, ListEntry** cur);

#define ListFindKeyM(head, retObj, key, objName, entryFieldName, keyFieldName) { \
	retObj = NULL; \
	ListEntry* cur = (head)->next; \
	while ((head) != cur) { \
		objName* tempObj = GetObjFromField(cur, objName, entryFieldName); \
		if (tempObj->keyFieldName == (key)) { \
			retObj = tempObj; \
			break; \
		} \
		cur = cur->next; \
	} \
}


#ifdef __cplusplus
}
#endif

#endif // UTILS_LIST_H_