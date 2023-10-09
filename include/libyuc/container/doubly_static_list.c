#include <libyuc/container/doubly_static_list.def>


void DoublyStaticListInit(DoublyStaticList* list, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Offset count) {
    list->list_first[0] = 0;
    LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id i = 0;
    for (; i < count - 1; i++) {
        LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetNext(list, &list->obj_arr[i], i + 1);
        if (i == 0) {
            LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetPrev(list, &list->obj_arr[i], LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Const_InvalidId);
        }
        else {
            LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetPrev(list, &list->obj_arr[i], i - 1);
        }
    }
    LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetNext(list, &list->obj_arr[i], LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Const_InvalidId);
    for (i = 1; i < LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_Const_StaticListCount; i++) {
        list->list_first[i] = LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Const_InvalidId;
    }
}
LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id DoublyStaticListDelete(DoublyStaticList* list, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id delete_index) {
    if (LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_GetPrev(list, &list->obj_arr[delete_index]) == LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Const_InvalidId) {
        list->list_first[list_order] = LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_GetNext(list, &list->obj_arr[delete_index]);
    }
    else {
        LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetNext(list, &list->obj_arr[LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_GetPrev(list, &list->obj_arr[delete_index])], LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_GetNext(list, &list->obj_arr[delete_index]));
    }
       
    if (LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_GetNext(list, &list->obj_arr[delete_index]) != LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Const_InvalidId) {
        LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetPrev(list, &list->obj_arr[LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_GetNext(list, &list->obj_arr[delete_index])], LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_GetPrev(list, &list->obj_arr[delete_index]));
    }
    return delete_index;
}
LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id DoublyStaticListPop(DoublyStaticList* list, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id list_order) {
    if (list->list_first[list_order] == LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Const_InvalidId) {
        return LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Const_InvalidId;
    }
    return DoublyStaticListDelete(list, list_order, list->list_first[list_order]);
}
void DoublyStaticListInsert(DoublyStaticList* list, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id prev_index, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id insert_index) {
    if (prev_index == LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Const_InvalidId) {
        LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetPrev(list, &list->obj_arr[insert_index], LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Const_InvalidId);
        LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetNext(list, &list->obj_arr[insert_index], list->list_first[list_order]);
        list->list_first[list_order] = insert_index;
    }
    else {
        LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetPrev(list, &list->obj_arr[insert_index], prev_index);
        LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetNext(list, &list->obj_arr[insert_index], LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_GetNext(list, &list->obj_arr[prev_index]));
        LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetNext(list, &list->obj_arr[prev_index], insert_index);
    }
    if (LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_GetNext(list, &list->obj_arr[insert_index]) != LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Const_InvalidId) {
        LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetPrev(list, &list->obj_arr[LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_GetNext(list, &list->obj_arr[insert_index])], insert_index);
    }
}
void DoublyStaticListPush(DoublyStaticList* list, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id push_index) {
        DoublyStaticListInsert(list, list_order, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Const_InvalidId, push_index);
}
void DoublyStaticListSwitch(DoublyStaticList* list, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id index, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id new_list_order) {
    DoublyStaticListDelete(list, list_order, index);
    DoublyStaticListPush(list, new_list_order, index);
}

#include <libyuc/container/doubly_static_list.undef>
