#ifndef CUTILS_SIGNLY_LIST_H_
#define CUTILS_SIGNLY_LIST_H_

#include "CUtils/container/object.h"
#include "CUtils/container/array.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _StaticListHead {
	Array array;
	int entryFieldOffset;
	int freeIndex;
} StaticListHead;

typedef struct _StaticListEntry {
	int nextIndex;
} StaticListEntry;

void StaticListHeadInit(StaticListHead* head, size_t count, int objSize, int entryFieldOffset);

void* StaticListAllocEntry(StaticListHead* head);

void StaticListFreeEntry(StaticListHead* head, int index);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_SIGNLY_LIST_H_