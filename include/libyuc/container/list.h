/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libyuc/container/list.def>

typedef struct ListEntry {
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id prev;
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id next;
}  ListEntry;

typedef struct ListHead {
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id last;
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id first;
} ListHead;



void ListInit(ListHead* head);
void ListPutEntryNext(ListHead* head, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id prev_id, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id);
void ListPutFirst(ListHead* head, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id);
void ListPutLast(ListHead* head, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id);
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListDeleteEntry(ListHead* head, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id);
void ListDeleteFirst(ListHead* head);
void ListDeleteLast(ListHead* head);
void ListReplaceEntry(ListHead* head, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id new_entry_id);
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Offset ListGetCount(ListHead* head);
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListFirst(ListHead* head);
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListLast(ListHead* head);
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListPrev(ListHead* head, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id cur_id);
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListNext(ListHead* head, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id cur_id);


#include <libyuc/container/list.undef>


#ifdef __cplusplus
}
#endif
