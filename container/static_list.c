/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/container/static_list.h>

void StaticListInit(StaticList* list, size_t count, int objSize, int entryFieldOffset) {
    Array* arr = &list->array;
    ArrayInit(arr, count, objSize);
    list->entryFieldOffset = entryFieldOffset;
    if (count == 0) {
        list->freeIndex = -1;
        return;
    }
    arr->count = count;
    list->freeIndex = 0;
    count--;
    int i = 0;
    for (; i < count; i++) {
        void* objEntry = ArrayAt(arr, i, void);
        StaticListEntry* listEntry = GetFieldByFieldOffset(objEntry, entryFieldOffset, StaticListEntry);
        listEntry->nextIndex = i + 1;
    }
    void* objEntry = ArrayAt(arr, i, void);
    StaticListEntry* listEntry = GetFieldByFieldOffset(objEntry, entryFieldOffset, StaticListEntry);
    listEntry->nextIndex = -1;
}

int StaticListAllocEntry(StaticList* list) {
    if (list->freeIndex == -1) {
        return NULL;
    }
    Array* arr = &list->array;
    void* objEntry = ArrayAt(arr, list->freeIndex, void);
    StaticListEntry* listEntry = GetFieldByFieldOffset(objEntry, list->entryFieldOffset, StaticListEntry);
    int allocIndex = list->freeIndex;
    list->freeIndex = listEntry->nextIndex;
    return allocIndex;
}

void StaticListFreeEntry(StaticList* list, int index) {
    Array* arr = &list->array;
    void* objEntry = ArrayAt(arr, index, void);
    StaticListEntry* listEntry = GetFieldByFieldOffset(objEntry, list->entryFieldOffset, StaticListEntry);
    listEntry->nextIndex = list->freeIndex;
    list->freeIndex = index;
}

int StaticListSwitchFreeIndex(StaticList* list, int newIndex) {
    int oldIndex = list->freeIndex;
    list->freeIndex = newIndex;
    return oldIndex;
}