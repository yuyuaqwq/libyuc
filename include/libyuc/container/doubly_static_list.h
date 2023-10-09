/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>


#include <libyuc/container/doubly_static_list.def>

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

#include <libyuc/container/doubly_static_list.undef>

