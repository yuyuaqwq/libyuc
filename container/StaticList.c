#include "StaticList.h"

void StaticListHeadInit(StaticListHead* head, size_t count, int objSize, int entryFieldOffset) {
	Array* arr = &head->array;
	ArrayInit(arr, count, objSize);
	head->entryFieldOffset = entryFieldOffset;
	if (count == 0) {
		head->freeIndex = -1;
		return;
	}
	arr->count = count;
	head->freeIndex = 0;
	count--;
	int i = 0;
	for (; i < count; i++) {
		void* objEntry = ArrayAt(arr, void, i);
		StaticListEntry* listEntry = GetFieldByFieldOffset(objEntry, StaticListEntry, entryFieldOffset);
		listEntry->nextIndex = i + 1;
	}
	*ArrayAt(arr, int, i) = -1;
}

void* StaticListAllocEntry(StaticListHead* head) {
	if (head->freeIndex == -1) {
		return NULL;
	}
	Array* arr = &head->array;
	void* objEntry = ArrayAt(arr, void, head->freeIndex);
	StaticListEntry* listEntry = GetFieldByFieldOffset(objEntry, StaticListEntry, head->entryFieldOffset);
	head->freeIndex = listEntry->nextIndex;
	return objEntry;
}

void StaticListFreeEntry(StaticListHead* head, int index) {
	Array* arr = &head->array;
	void* objEntry = ArrayAt(arr, void, index);
	StaticListEntry* listEntry = GetFieldByFieldOffset(objEntry, StaticListEntry, head->entryFieldOffset);
	listEntry->nextIndex = head->freeIndex;
	head->freeIndex = index;
}
