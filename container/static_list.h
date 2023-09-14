/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_STATIC_LIST_H_
#define LIBYUC_CONTAINER_STATIC_LIST_H_

#include <libyuc/basic.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME
#define LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME
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


typedef struct StaticListEntry {
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id next;
} StaticListEntry;

typedef struct StaticList {
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_first[list_count]; /* 最少应为1 */
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Element obj_arr[obj_arr_count];
} StaticList;
   
void StaticListInit(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id count);
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListPop(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order);
void StaticListPush(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id index);
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListDelete(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id prev_id, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id delete_id);
void StaticListSwitch(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id prev_id, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id id, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id new_list_order);
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListIteratorFirst(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order);
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListIteratorNext(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id cur_id);


/*
* 访问器需要提供_GetNext、_SetNext方法
*/

void StaticListInit(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id count) {
    list->list_first[0] = 0;
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id i = 0;
    for (; i < count - 1; i++) {
         LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_SetNext(list, &list->obj_arr[i], i + 1);
    }
     LIBYUC_CONTAINER_STATIC_LIST_ACCESSOR_SetNext(list, &list->obj_arr[i], LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Const_InvalidId);
       
    for (i = 1; i < list_count; i++) {
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



#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_STATIC_LIST_H_