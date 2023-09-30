/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libyuc/container/singly_list.def>

typedef struct SinglyListEntry {
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id next;
} SinglyListEntry;
typedef struct SinglyListHead {
    LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id first;
} SinglyListHead;
   

void SinglyListHeadInit(SinglyListHead* head);
bool SinglyListIsEmpty(SinglyListHead* head);
void SinglyListPutFirst(SinglyListHead* head, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id entry_id);
void SinglyListDeleteFirst(SinglyListHead* head);
void SinglyListDeleteEntry(SinglyListHead* head, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id prev_id, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id del_entry_id);
LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Offset SinglyListGetCount(SinglyListHead* head);
LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id SinglyListIteratorFirst(SinglyListHead* head);
LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id SinglyListIteratorNext(SinglyListHead* head, LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id entry_id);


#include <libyuc/container/singly_list.undef>

#ifdef __cplusplus
}
#endif
