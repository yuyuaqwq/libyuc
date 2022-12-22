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

bool ListTraversal(ListEntry* head, ListEntry** cur);

#define ListFindKey_Macro(head, retEntry, key, typeName, entryFieldName, keyFieldName) { \
	ListEntry* cur; \
	retEntry = NULL; \
	while (ListTraversal((head), &cur)) { \
		typeName* temp = GetHeadFromField(cur, typeName, entryFieldName); \
		if (temp->keyFieldName == (key)) { \
			retEntry = (typeName*)cur; \
			break; \
		} \
	} \
}


#ifdef __cplusplus
}
#endif

#endif // UTILS_LIST_H_