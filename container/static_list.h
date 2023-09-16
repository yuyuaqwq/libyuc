/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/


#include <libyuc/basic.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME
#define LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME
#endif

#ifndef LIBYUC_CONTAINER_STATIC_LIST_Const_StaticElementCount
#define LIBYUC_CONTAINER_STATIC_LIST_Const_StaticElementCount 0
#endif

#ifndef LIBYUC_CONTAINER_STATIC_LIST_Const_StaticListCount
#define LIBYUC_CONTAINER_STATIC_LIST_Const_StaticListCount 1
#endif

#ifndef LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_DynamicArray
#define LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_DynamicArray LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Element*
#endif

#ifndef  LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_GetNext
#define LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_GetNext(main_obj, obj) ((obj)->next)
#endif

#ifndef  LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_SetNext
#define LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_SetNext(main_obj, obj, new_next) ((obj)->next = (new_next))
#endif

#ifndef LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Element
#define LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Element StaticListEntry
#endif

#ifndef LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id
#define LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id size_t
#endif

#ifndef LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Offset
#define LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Offset LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id
#endif

#ifndef LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Const_InvalidId
#define LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Const_InvalidId -1
#endif


#define StaticListEntry MAKE_NAME(LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME, StaticListEntry)
#define StaticList MAKE_NAME(LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME, StaticList)


typedef struct StaticListEntry {
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id next;
} StaticListEntry;

#define LIBYUC_CONTAINER_STATIC_LIST_MODE_STATIC

#ifdef LIBYUC_CONTAINER_STATIC_LIST_MODE_STATIC
typedef struct StaticList {
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_first[LIBYUC_CONTAINER_STATIC_LIST_Const_StaticListCount];
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Element obj_arr[LIBYUC_CONTAINER_STATIC_LIST_Const_StaticElementCount];
} StaticList;
#endif

#ifdef LIBYUC_CONTAINER_STATIC_LIST_MODE_DYNAMIC
typedef struct StaticList {
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_first[LIBYUC_CONTAINER_STATIC_LIST_Const_StaticListCount];
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_DynamicArray obj_arr;
} StaticList;
#endif

#define StaticListInit MAKE_NAME(LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME, StaticListInit)
#define StaticListPop MAKE_NAME(LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME, StaticListPop)
#define StaticListPush MAKE_NAME(LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME, StaticListPush)
#define StaticListDelete MAKE_NAME(LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME, StaticListDelete)
#define StaticListSwitch MAKE_NAME(LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME, StaticListSwitch)
#define StaticListIteratorFirst MAKE_NAME(LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME, StaticListIteratorFirst)
#define StaticListIteratorNext MAKE_NAME(LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME, StaticListIteratorNext)
#define StaticListExpand MAKE_NAME(LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME, StaticListExpand)

void StaticListInit(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Offset count);
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListPop(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order);
void StaticListPush(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id index);
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListDelete(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id prev_id, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id delete_id);
void StaticListSwitch(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id prev_id, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id id, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id new_list_order);
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListIteratorFirst(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order);
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListIteratorNext(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id cur_id);
void StaticListExpand(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id old_count, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id new_count);

#ifdef LIBYUC_TEMPLATE_C_INCLUDE

void StaticListInit(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id count) {
    list->list_first[0] = 0;
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id i = 0;
    for (; i < count - 1; i++) {
         LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_SetNext(list, &list->obj_arr[i], i + 1);
    }
     LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_SetNext(list, &list->obj_arr[i], LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Const_InvalidId);
       
    for (i = 1; i < LIBYUC_CONTAINER_STATIC_LIST_Const_StaticListCount; i++) {
        list->list_first[i] = LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Const_InvalidId;
    }
}
void StaticListExpand(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id old_count, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id new_count) {
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id old_first = list->list_first[0];
    list->list_first[0] = new_count - 1;
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id i = old_count;
    for (; i < new_count - 1; i++) {
         LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_SetNext(list, &list->obj_arr[i], i + 1);
    }
     LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_SetNext(list, &list->obj_arr[i], old_first);
}
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListPop(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order) {
    if (list->list_first[list_order] == LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Const_InvalidId) {
        return LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Const_InvalidId;
    }
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id index = list->list_first[list_order];
    list->list_first[list_order] =  LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_GetNext(list, &list->obj_arr[index]);
    return index;
}
void StaticListPush(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id index) {
     LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_SetNext(list, &list->obj_arr[index], list->list_first[list_order]);
    list->list_first[list_order] = index;
}
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListDelete(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id prev_id, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id delete_id) {
    if (prev_id == LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Const_InvalidId) {
        list->list_first[list_order] =  LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_GetNext(list, &list->obj_arr[delete_id]);
    }
    else {
         LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_SetNext(list, &list->obj_arr[prev_id],  LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_GetNext(list, &list->obj_arr[delete_id]));
    }
    return delete_id;
}
void StaticListSwitch(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id prev_id, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id id, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id new_list_order) {
    StaticListDelete(list, list_order, prev_id, id);
    StaticListPush(list, new_list_order, id);
}
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListIteratorFirst(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order) {
    return list->list_first[list_order];
}
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListIteratorNext(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id cur_id) {
    return  LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_GetNext(list, &list->obj_arr[cur_id]);
}

#endif

#undef StaticListEntry
#undef StaticList


#undef StaticListInit
#undef StaticListPop
#undef StaticListPush
#undef StaticListDelete
#undef StaticListSwitch
#undef StaticListIteratorNext
#undef StaticListExpand


#undef LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME
#undef LIBYUC_CONTAINER_STATIC_LIST_Const_StaticListCount
#undef LIBYUC_CONTAINER_STATIC_LIST_Const_StaticElementCount
#undef LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Const_InvalidId
#undef LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Element
#undef LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id
#undef LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Offset
#undef LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_DynamicArray

#undef LIBYUC_CONTAINER_STATIC_LIST_MODE_STATIC
#undef LIBYUC_CONTAINER_STATIC_LIST_MODE_DYNAMIC

#ifdef __cplusplus
}
#endif
