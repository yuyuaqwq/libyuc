/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifndef LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_CLASS_NAME
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_CLASS_NAME
#endif

#ifndef LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_Const_StaticElementCount
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_Const_StaticElementCount 0
#endif

#ifndef LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_Const_StaticListCount
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_Const_StaticListCount 1
#endif

#ifndef LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_DynamicArray
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_DynamicArray LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Element*
#endif

#ifndef  LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_GetNext
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_GetNext(main_obj, obj) ((obj)->next)
#endif

#ifndef  LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetNext
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetNext(main_obj, obj, new_next) ((obj)->next = (new_next))
#endif

#ifndef  LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_GetPrev
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_GetPrev(main_obj, obj) ((obj)->prev)
#endif

#ifndef  LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetPrev
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_ACCESSOR_SetPrev(main_obj, obj, new_prev) ((obj)->prev = (new_prev))
#endif

#ifndef LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Element
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Element DoublyStaticListEntry
#endif

#ifndef LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id size_t
#endif

#ifndef LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Offset
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Offset LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id
#endif

#ifndef LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Const_InvalidId
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Const_InvalidId -1
#endif


/*
* 若只有一条队列，由Pop分配的节点，此结构体可以被覆盖，可以使用union
*/
typedef struct DoublyStaticListEntry{
    LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id prev;
    LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id next;
} DoublyStaticListEntry;

typedef struct DoublyStaticList{
    LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id list_first[LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_Const_StaticListCount];
    LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Element obj_arr[LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_Const_StaticListCount];
} DoublyStaticList;
   
void DoublyStaticListInit(DoublyStaticList* list, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Offset count);
LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id DoublyStaticListPop(DoublyStaticList* list, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id list_order);
void DoublyStaticListPush(DoublyStaticList* list, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id index);
void DoublyStaticListSwitch(DoublyStaticList* list, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id index, LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_INDEXER_Type_Id new_list_order);



/*
* 访问器需要提供_GetNext、_SetNext、_GetPrev、_SetPrev方法
*/

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



#ifdef __cplusplus
}
#endif
