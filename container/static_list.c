/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/container/static_list.h>

const int kStaticListInvalidIndex = -1;

void StaticListInitFromBuf(StaticList* list, void* buf, size_t count, int obj_size, int entry_field_offset, int list_count) {
    list->list_first[0] = 0;
    int i = 0;
    for (; i < count; i++) {
        void* obj_entry = ObjectArrayAt(buf, i, obj_size);
        StaticListEntry* list_entry = ObjectGetFieldByOffset(obj_entry, entry_field_offset, StaticListEntry);
        if (i + 1 == count) {
            list_entry->next_index = kStaticListInvalidIndex;
        }
        else {
            list_entry->next_index = i + 1;
        }
    }

    for (i = 1; i < list_count; i++) {
        list->list_first[i] = kStaticListInvalidIndex;
    }
}

void StaticListInit(StaticList* list, size_t count, int obj_size, int entry_field_offset, int list_count) {
    Array* arr = &list->array;
    ArrayInit(arr, count, obj_size);
    list->entry_field_offset = entry_field_offset;
    if (count == 0) {
        list->list_first[0] = kStaticListInvalidIndex;
        return;
    }
    arr->count = count;
    StaticListInitFromBuf(list, ArrayGetData(arr), count, obj_size, entry_field_offset, list_count);
}

int StaticListPop(StaticList* list, int list_order) {
    if (list->list_first[list_order] == kStaticListInvalidIndex) {
        return kStaticListInvalidIndex;
    }
    int index = list->list_first[list_order];
    Array* arr = &list->array;
    void* obj_entry = ArrayAt(arr, index, void);
    StaticListEntry* list_entry = ObjectGetFieldByOffset(obj_entry, list->entry_field_offset, StaticListEntry);
    list->list_first[list_order] = list_entry->next_index;
    return index;
}

void StaticListPush(StaticList* list, int list_order, int index) {
    Array* arr = &list->array;
    void* obj_entry = ArrayAt(arr, index, void);
    StaticListEntry* list_entry = ObjectGetFieldByOffset(obj_entry, list->entry_field_offset, StaticListEntry);
    list_entry->next_index = list->list_first[list_order];
    list->list_first[list_order] = index;
}