/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>

#include <libyuc/container/list.def>


typedef struct ListEntry {
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id prev;
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id next;
}  ListEntry;

typedef struct List {
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id last;
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id first;
} List;



void ListInit(List* list);
void ListPutFirst(List* list, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id);
void ListPutLast(List* list, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id);
void ListPutNext(List* list, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id prev_id, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id);
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListDeleteFirst(List* list);
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListDeleteLast(List* list);
void ListDelete(List* list, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id);
void ListReplace(List* list, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id new_entry_id);

LIBYUC_CONTAINER_LIST_REFERENCER_Type_Offset ListGetCount(List* list);

LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListFirst(List* list);
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListLast(List* list);
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListPrev(List* list, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id);
LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id ListNext(List* list, LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id entry_id);

#include <libyuc/container/list.undef>

