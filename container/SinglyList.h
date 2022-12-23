#ifndef UTILS_SIGNLY_LIST_H_
#define UTILS_SIGNLY_LIST_H_

#include "CUtils/container/object.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SinglyListEntry {
	struct _SinglyListEntry* next;
} SinglyListEntry;


void SinglyListHeadInit(SinglyListEntry* head);

bool SinglyListIsEmpty(SinglyListEntry* head);

void SinglyListInsertHead(SinglyListEntry* head, SinglyListEntry* entry);

SinglyListEntry* SinglyListRemoveHead(SinglyListEntry* head);

size_t SinglyListEntryCount(SinglyListEntry* head);

bool SinglyListIteration(SinglyListEntry* head, SinglyListEntry** cur);

#define SinglyListFindKeyM(head, retObj, key, objName, entryFieldName, keyFieldName) { \
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

#endif // UTILS_SIGNLY_LIST_H_