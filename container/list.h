/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_LIST_H_
#define LIBYUC_CONTAINER_LIST_H_

#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CLASS_NAME
#define CLASS_NAME
#endif

#ifndef REFERENCER_Type_Id
#define REFERENCER_Type_Id struct ListEntry*
#endif

#ifndef REFERENCER_Type_Offset
#define REFERENCER_Type_Offset size_t
#endif


typedef struct MAKE_NAME(CLASS_NAME, ListEntry) {
    REFERENCER_Type_Id prev;
    REFERENCER_Type_Id next;
}  MAKE_NAME(CLASS_NAME, ListEntry);

typedef struct MAKE_NAME(CLASS_NAME, ListHead) {
    REFERENCER_Type_Id last;
    REFERENCER_Type_Id first;
} MAKE_NAME(CLASS_NAME, ListHead);

#define ListEntry MAKE_NAME(CLASS_NAME, ListEntry)
#define ListHead MAKE_NAME(CLASS_NAME, ListHead)

void MAKE_NAME(CLASS_NAME, ListInit)(ListHead* head);
void MAKE_NAME(CLASS_NAME, ListPutEntryNext)(ListHead* head, REFERENCER_Type_Id prev_id, REFERENCER_Type_Id entry_id);
void MAKE_NAME(CLASS_NAME, ListPutFirst)(ListHead* head, REFERENCER_Type_Id entry_id);
void MAKE_NAME(CLASS_NAME, ListPutLast)(ListHead* head, REFERENCER_Type_Id entry_id);
REFERENCER_Type_Id MAKE_NAME(CLASS_NAME, ListDeleteEntry)(ListHead* head, REFERENCER_Type_Id entry_id);
void MAKE_NAME(CLASS_NAME, ListDeleteFirst)(ListHead* head);
void MAKE_NAME(CLASS_NAME, ListDeleteLast)(ListHead* head);
void MAKE_NAME(CLASS_NAME, ListReplaceEntry)(ListHead* head, REFERENCER_Type_Id entry_id, REFERENCER_Type_Id new_entry_id);
REFERENCER_Type_Offset MAKE_NAME(CLASS_NAME, ListGetCount)(ListHead* head);
REFERENCER_Type_Id MAKE_NAME(CLASS_NAME, ListFirst)(ListHead* head);
REFERENCER_Type_Id MAKE_NAME(CLASS_NAME, ListLast)(ListHead* head);
REFERENCER_Type_Id MAKE_NAME(CLASS_NAME, ListPrev)(ListHead* head, REFERENCER_Type_Id cur_id);
REFERENCER_Type_Id MAKE_NAME(CLASS_NAME, ListNext)(ListHead* head, REFERENCER_Type_Id cur_id);

#undef ListEntry
#undef ListHead

#undef CLASS_NAME
#undef REFERENCER_Type_Id
#undef REFERENCER_Type_Offset

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_LIST_H_