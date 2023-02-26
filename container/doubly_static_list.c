/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#include <CUtils/container/doubly_static_list.h>

const int kDoublyStaticListInvalidIndex = -1;

void DoublyStaticListInit(DoublyStaticList* list, size_t count, int obj_size, int entry_field_offset, int list_count) {
    Array* arr = &list->array;
    ArrayInit(arr, count, obj_size);
    list->entry_field_offset = entry_field_offset;
    if (count == 0) {
        list->list_first[0] = kDoublyStaticListInvalidIndex;
        return;
    }
    arr->count = count;
    list->list_first[0] = 0;
    count--;
    int i = 0;
    for (; i < count; i++) {
        void* obj_entry = ArrayAt(arr, i, void);
        DoublyStaticListEntry* list_entry = GetFieldByFieldOffset(obj_entry, entry_field_offset, DoublyStaticListEntry);
        if (i + 1 == count) {
            list_entry->next_index = kDoublyStaticListInvalidIndex;
        } else {
            list_entry->next_index = i + 1;
        }
        if (i == 0) {
            list_entry->prev_index = kDoublyStaticListInvalidIndex;
        } else {
            list_entry->prev_index = i - 1;
        }
    }

    for (i = 1; i < list_count; i++) {
        list->list_first[i] = kDoublyStaticListInvalidIndex;
    }
}

int DoublyStaticListEntryRemove(DoublyStaticList* list, int list_order, int remove_index) {
    Array* arr = &list->array;
    void* remove_obj_entry = ArrayAt(arr, remove_index, void);
    DoublyStaticListEntry* list_entry = GetFieldByFieldOffset(remove_obj_entry, list->entry_field_offset, DoublyStaticListEntry);
    if (list_entry->prev_index == kDoublyStaticListInvalidIndex) {
        list->list_first[list_order] = list_entry->next_index;
    }
    else {
        void* prev_obj_entry = ArrayAt(arr, list_entry->prev_index, void);
        DoublyStaticListEntry* prev_list_entry = GetFieldByFieldOffset(remove_obj_entry, list->entry_field_offset, DoublyStaticListEntry);
        prev_list_entry->next_index = list_entry->next_index;
    }

    if (list_entry->next_index != kDoublyStaticListInvalidIndex) {
        void* next_obj_entry = ArrayAt(arr, list_entry->next_index, void);
        DoublyStaticListEntry* next_list_entry = GetFieldByFieldOffset(remove_obj_entry, list->entry_field_offset, DoublyStaticListEntry);
        next_list_entry->prev_index = list_entry->prev_index;
    }
}

int DoublyStaticListEntryPop(DoublyStaticList* list, int list_order) {
    if (list->list_first[list_order] == kDoublyStaticListInvalidIndex) {
        return kDoublyStaticListInvalidIndex;
    }
    return DoublyStaticListEntryRemove(list, list_order, list->list_first[list_order]);
}

void DoublyStaticListEntryInsertNext(DoublyStaticList* list, int list_order, int prev_index, int insert_index) {
    Array* arr = &list->array;
    void* insert_obj_entry = ArrayAt(arr, insert_index, void);
    DoublyStaticListEntry* insert_list_entry = GetFieldByFieldOffset(insert_obj_entry, list->entry_field_offset, DoublyStaticListEntry);

    if (prev_index == list->list_first[list_order]) {
        insert_list_entry->prev_index = kDoublyStaticListInvalidIndex;
        insert_list_entry->next_index = list->list_first[list_order];
        list->list_first[list_order] = insert_index;
    }
    else {
        insert_list_entry->prev_index = prev_index;
        void* prev_obj_entry = ArrayAt(arr, prev_index, void);
        DoublyStaticListEntry* prev_list_entry = GetFieldByFieldOffset(prev_obj_entry, list->entry_field_offset, DoublyStaticListEntry);
        insert_list_entry->next_index = prev_list_entry->next_index;
        prev_list_entry->next_index = insert_index;
    }
    if (insert_list_entry->next_index != kDoublyStaticListInvalidIndex) {
        void* next_obj_entry = ArrayAt(arr, insert_list_entry->next_index, void);
        DoublyStaticListEntry* next_list_entry = GetFieldByFieldOffset(next_obj_entry, list->entry_field_offset, DoublyStaticListEntry);
        next_list_entry->prev_index = insert_index;
    }
}

void DoublyStaticListEntryPush(DoublyStaticList* list, int list_order, int push_index) {
    DoublyStaticListEntryInsertNext(list, list_order, list->list_first[list_order], push_index);
}