#ifndef CUTILS_SIGNLY_LIST_H_
#define CUTILS_SIGNLY_LIST_H_

#include "CUtils/container/object.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SinglyListEntry {
	struct _SinglyListEntry* next;
} SinglyListEntry;

typedef SinglyListEntry SinglyListHead;

void SinglyListHeadInit(SinglyListHead* head);

bool SinglyListIsEmpty(SinglyListHead* head);

void SinglyListInsertHead(SinglyListHead* head, SinglyListEntry* entry);

SinglyListEntry* SinglyListRemoveHead(SinglyListHead* head);

size_t SinglyListGetCount(SinglyListHead* head);

bool SinglyListIteration(SinglyListHead* head, SinglyListEntry** cur);

#define SinglyListFindEntryByKeyM(head, retObj, key, objName, entryFieldName, keyFieldName) { \
	retObj = NULL; \
	SinglyListEntry* cur = (head)->next; \
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

#endif // CUTILS_SIGNLY_LIST_H_