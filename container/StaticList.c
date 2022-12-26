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
		void* objEntry = ArrayAt(arr, i, void);
		StaticListEntry* listEntry = GetFieldByFieldOffset(objEntry, StaticListEntry, entryFieldOffset);
		listEntry->nextIndex = i + 1;
	}
	void* objEntry = ArrayAt(arr, i, void);
	StaticListEntry* listEntry = GetFieldByFieldOffset(objEntry, StaticListEntry, entryFieldOffset);
	listEntry->nextIndex = -1;
}

int StaticListAllocEntry(StaticListHead* head) {
	if (head->freeIndex == -1) {
		return NULL;
	}
	Array* arr = &head->array;
	void* objEntry = ArrayAt(arr, head->freeIndex, void);
	StaticListEntry* listEntry = GetFieldByFieldOffset(objEntry, StaticListEntry, head->entryFieldOffset);
	int allocIndex = head->freeIndex;
	head->freeIndex = listEntry->nextIndex;
	return allocIndex;
}

void StaticListFreeEntry(StaticListHead* head, int index) {
	Array* arr = &head->array;
	void* objEntry = ArrayAt(arr, index, void);
	StaticListEntry* listEntry = GetFieldByFieldOffset(objEntry, StaticListEntry, head->entryFieldOffset);
	listEntry->nextIndex = head->freeIndex;
	head->freeIndex = index;
}

int StaticListSwitchFreeIndex(StaticListHead* head, int newIndex) {
	int oldIndex = head->freeIndex;
	head->freeIndex = newIndex;
	return oldIndex;
}