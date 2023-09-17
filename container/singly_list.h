/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_SINGLY_SINGLY_LIST_H_
#define LIBYUC_CONTAINER_SINGLY_SINGLY_LIST_H_

#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifndef LIBYUC_CONTAINER_SINGLY_LIST_CLASS_NAME
#define LIBYUC_CONTAINER_SINGLY_LIST_CLASS_NAME
#endif

#ifndef LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id
#define LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id struct SinglyListEntry*
#endif

#ifndef LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Offset
#define LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Offset size_t
#endif

#ifndef LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Const_InvalidId
#define LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Const_InvalidId NULL
#endif

#ifndef LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference
#define LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference(main_obj, reference) (reference) 
#endif

#ifndef LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference
#define LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference(main_obj, reference)
#endif

#define SinglyListHead MAKE_NAME(LIBYUC_CONTAINER_SINGLY_LIST_CLASS_NAME, SinglyListHead)
#define SinglyListEntry MAKE_NAME(LIBYUC_CONTAINER_SINGLY_LIST_CLASS_NAME, SinglyListEntry)

typedef struct SinglyListEntry {
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id next;
} SinglyListEntry;
typedef struct SinglyListHead {
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id first;
} SinglyListHead;
   
#define SinglyListHeadInit MAKE_NAME(LIBYUC_CONTAINER_SINGLY_LIST_CLASS_NAME, SinglyListHeadInit)
#define SinglyListIsEmpty MAKE_NAME(LIBYUC_CONTAINER_SINGLY_LIST_CLASS_NAME, SinglyListIsEmpty)
#define SinglyListPutFirst MAKE_NAME(LIBYUC_CONTAINER_SINGLY_LIST_CLASS_NAME, SinglyListPutFirst)
#define SinglyListDeleteFirst MAKE_NAME(LIBYUC_CONTAINER_SINGLY_LIST_CLASS_NAME, SinglyListDeleteFirst)
#define SinglyListDeleteEntry MAKE_NAME(LIBYUC_CONTAINER_SINGLY_LIST_CLASS_NAME, SinglyListDeleteEntry)
#define SinglyListGetCount MAKE_NAME(LIBYUC_CONTAINER_SINGLY_LIST_CLASS_NAME, SinglyListGetCount)
#define SinglyListIteratorFirst MAKE_NAME(LIBYUC_CONTAINER_SINGLY_LIST_CLASS_NAME, SinglyListIteratorFirst)
#define SinglyListIteratorNext MAKE_NAME(LIBYUC_CONTAINER_SINGLY_LIST_CLASS_NAME, SinglyListIteratorNext)

void SinglyListHeadInit(SinglyListHead* head);
bool SinglyListIsEmpty(SinglyListHead* head);
void SinglyListPutFirst(SinglyListHead* head, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id entry_id);
void SinglyListDeleteFirst(SinglyListHead* head);
void SinglyListDeleteEntry(SinglyListHead* head, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id prev_id, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id del_entry_id);
LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Offset SinglyListGetCount(SinglyListHead* head);
LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id SinglyListIteratorFirst(SinglyListHead* head);
LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id SinglyListIteratorNext(SinglyListHead* head, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id entry_id);

#ifdef LIBYUC_TEMPLATE_DEFINE

void SinglyListHeadInit(SinglyListHead* head) {
    head->first = LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Const_InvalidId;
}
bool SinglyListIsEmpty(SinglyListHead* head) {
    return head->first == LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Const_InvalidId;
}
void SinglyListPutFirst(SinglyListHead* head, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id entry_id) {
    SinglyListEntry* entry = LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference(head, entry_id);
    entry->next = head->first;
    head->first = entry_id;
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference(head, entry);
}
void SinglyListDeleteFirst(SinglyListHead* head) {
    SinglyListEntry* old_first = LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference(head, head->first);
    head->first = old_first->next;
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference(head, old_first);
}
void SinglyListDeleteEntry(SinglyListHead* head, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id prev_id, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id del_entry_id) {
    SinglyListEntry* prev;
    if (prev_id != LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Const_InvalidId) {
        prev = LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference(head, prev_id);
    }
    else {
        prev = (SinglyListEntry*)head;
    }
    SinglyListEntry* del_entry = LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference(head, del_entry_id);
    prev->next = del_entry->next;
    if (prev_id != LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Const_InvalidId) {
        LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference(head, prev);
    }
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference(head, del_entry);
}
LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Offset SinglyListGetCount(SinglyListHead* head) {
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Offset count = 0;
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id cur_id = head->first;
    while (cur_id != LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Const_InvalidId) {
        count++;
        SinglyListEntry* cur = LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference(head, cur_id);
        cur_id = cur->next;
        LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference(head, cur);
    }
    return count;
}
LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id SinglyListIteratorFirst(SinglyListHead* head) {
    return head->first;
}
LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id SinglyListIteratorNext(SinglyListHead* head, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id entry_id) {
    SinglyListEntry* entry = LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference(head, entry_id);
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id next_id = entry->next;
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference(head, entry);
    return next_id;
}
#endif

#undef SinglyListEntry
#undef SinglyListHead

#undef SinglyListHeadInit
#undef SinglyListIsEmpty
#undef SinglyListPutFirst
#undef SinglyListDeleteFirst
#undef SinglyListDeleteEntry
#undef SinglyListGetCount
#undef SinglyListIteratorFirst
#undef SinglyListIteratorNext


#undef LIBYUC_CONTAINER_SINGLY_LIST_CLASS_NAME
#undef LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id
#undef LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Offset
#undef LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Const_InvalidId
#undef LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference
#undef LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Dereference

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_SINGLY_SINGLY_LIST_H_