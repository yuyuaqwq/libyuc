/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/


#include <libyuc/basic.h>

#include <libyuc/container/static_list.def>

typedef struct StaticListEntry {
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id next;
} StaticListEntry;


typedef struct StaticList {
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_first[LIBYUC_CONTAINER_STATIC_LIST_Const_StaticListCount];
#if defined(LIBYUC_CONTAINER_STATIC_LIST_MODE_STATIC)
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Element obj_arr[LIBYUC_CONTAINER_STATIC_LIST_Const_StaticElementCount];
#elif defined(LIBYUC_CONTAINER_STATIC_LIST_MODE_DYNAMIC)
    LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_DynamicArray obj_arr;
#endif
} StaticList;


void StaticListInit(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Offset count);
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListPop(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order);
void StaticListPush(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id index);
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListDelete(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id prev_id, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id delete_id);
void StaticListSwitch(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id prev_id, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id id, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id new_list_order);
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListIteratorFirst(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id list_order);
LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id StaticListIteratorNext(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id cur_id);
void StaticListExpand(StaticList* list, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id old_count, LIBYUC_CONTAINER_STATIC_LIST_INDEXER_Type_Id new_count);



#include <libyuc/container/static_list.undef>

