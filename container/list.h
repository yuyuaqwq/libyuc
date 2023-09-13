/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_LIST_H_
#define LIBYUC_CONTAINER_LIST_H_

#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBYUC_CONTAINER_LIST_CLASS_NAME
#define LIBYUC_CONTAINER_LIST_CLASS_NAME
#endif

#ifndef LIBYUC_CONTAINER_LIST_LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id
#define LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id struct ListEntry*
#endif

#ifndef LIBYUC_CONTAINER_LIST_REFERENCER_Type_Offset
#define LIBYUC_CONTAINER_LIST_REFERENCER_Type_Offset size_t
#endif

#ifndef LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId
#define LIBYUC_CONTAINER_LIST_REFERENCER_Const_InvalidId NULL
#endif

#ifndef LIBYUC_CONTAINER_LIST_REFERENCER_Reference
#define LIBYUC_CONTAINER_LIST_REFERENCER_Reference(main_obj, reference) (reference) 
#endif

#ifndef LIBYUC_CONTAINER_LIST_REFERENCER_Dereference
#define LIBYUC_CONTAINER_LIST_REFERENCER_Dereference(main_obj, reference)
#endif


#define ListEntry MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListEntry)
#define ListHead MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListHead)

typedef struct ListEntry {
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id prev;
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id next;
}  ListEntry;

typedef struct ListHead {
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id last;
    LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id first;
} ListHead;


#define ListInit MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListInit)
#define ListPutEntryNext MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListPutEntryNext)
#define ListPutFirst MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListPutFirst)
#define ListPutLast MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListPutLast)
#define ListDeleteEntry MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListDeleteEntry)
#define ListDeleteFirst MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListDeleteFirst)
#define ListDeleteLast MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListDeleteLast)
#define ListReplaceEntry MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListReplaceEntry)
#define ListGetCount MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListGetCount)
#define ListFirst MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListFirst)
#define ListLast MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListLast)
#define ListPrev MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListPrev)
#define ListNext MAKE_NAME(LIBYUC_CONTAINER_LIST_CLASS_NAME, ListNext)


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

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_LIST_H_


#ifndef LIBYUC_TEMPLATE_C_INCLUDE
#undef ListEntry
#undef ListHead

#undef ListInit
#undef ListPutFirst
#undef ListPutLast
#undef ListDeleteEntry
#undef ListDeleteFirst
#undef ListDeleteLast
#undef ListReplaceEntry
#undef ListGetCount
#undef ListFirst
#undef ListLast
#undef ListPrev
#undef ListNext

#undef LIBYUC_CONTAINER_LIST_CLASS_NAME
#undef LIBYUC_CONTAINER_LIST_REFERENCER_Type_Id
#undef LIBYUC_CONTAINER_LIST_REFERENCER_Type_Offset
#undef LIBYUC_CONTAINER_LIST_REFERENCER_Reference
#undef LIBYUC_CONTAINER_LIST_REFERENCER_Dereference
#endif