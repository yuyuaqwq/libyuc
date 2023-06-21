/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef CUTILS_CONTAINER_SINGLY_LIST_H_
#define CUTILS_CONTAINER_SINGLY_LIST_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif


#define CUTILS_CONTAINER_SINGLY_LIST_DECLARATION(singly_list_type_name, id_type) \
    typedef struct _##singly_list_type_name##SinglyListEntry { \
        id_type next; \
    } singly_list_type_name##SinglyListEntry; \
    typedef struct _##singly_list_type_name##SinglyListHead { \
        id_type first; \
    } singly_list_type_name##SinglyListHead; \
    \
    void singly_list_type_name##SinglyListHeadInit(singly_list_type_name##SinglyListHead* head); \
    bool singly_list_type_name##SinglyListIsEmpty(singly_list_type_name##SinglyListHead* head); \
    void singly_list_type_name##SinglyListPutFirst(singly_list_type_name##SinglyListHead* head, id_type entry_id); \
    id_type singly_list_type_name##SinglyListDeleteFirst(singly_list_type_name##SinglyListHead* head); \
    id_type singly_list_type_name##SinglyListDeleteEntry(singly_list_type_name##SinglyListHead* head, id_type prev_id, id_type del_entry_id); \
    size_t singly_list_type_name##SinglyListGetCount(singly_list_type_name##SinglyListHead* head); \
    id_type singly_list_type_name##SinglyListIteratorFirst(singly_list_type_name##SinglyListHead* head); \
    id_type singly_list_type_name##SinglyListIteratorNext(singly_list_type_name##SinglyListHead* head, id_type entry_id); \

#define CUTILS_CONTAINER_SINGLY_LIST_DEFINE(singly_list_type_name, id_type, referencer) \
    void singly_list_type_name##SinglyListHeadInit(singly_list_type_name##SinglyListHead* head) { \
        head->first = referencer##_InvalidId; \
    } \
    bool singly_list_type_name##SinglyListIsEmpty(singly_list_type_name##SinglyListHead* head) { \
        return head->first == referencer##_InvalidId; \
    } \
    void singly_list_type_name##SinglyListPutFirst(singly_list_type_name##SinglyListHead* head, id_type entry_id) { \
        singly_list_type_name##SinglyListEntry* entry = referencer##_Reference(head, entry_id); \
        id_type old_first_id = head->first; \
        head->first = entry_id; \
        entry->next = old_first_id; \
        referencer##_Dereference(head, entry); \
    } \
    id_type singly_list_type_name##SinglyListDeleteFirst(singly_list_type_name##SinglyListHead* head) { \
        id_type old_first_id = head->first; \
        singly_list_type_name##SinglyListEntry* old_first = referencer##_Reference(head, old_first_id); \
        head->first = old_first->next; \
        referencer##_Dereference(head, old_first_id); \
        return old_first_id; \
    } \
    id_type singly_list_type_name##SinglyListDeleteEntry(singly_list_type_name##SinglyListHead* head, id_type prev_id, id_type del_entry_id) { \
        singly_list_type_name##SinglyListEntry* prev = referencer##_Reference(head, prev_id); \
        singly_list_type_name##SinglyListEntry* del_entry = referencer##_Reference(head, del_entry_id); \
        prev->next = del_entry->next; \
        referencer##_Dereference(head, prev); \
        referencer##_Dereference(head, del_entry); \
        return del_entry_id; \
    } \
    size_t singly_list_type_name##SinglyListGetCount(singly_list_type_name##SinglyListHead* head) { \
        size_t count = 0; \
        id_type cur_id = head->first; \
        while (cur_id != referencer##_InvalidId) { \
            count++; \
            singly_list_type_name##SinglyListEntry* cur = referencer##_Reference(head, cur_id); \
            cur_id = cur->next; \
            referencer##_Dereference(head, cur); \
        } \
        return count; \
    } \
    id_type singly_list_type_name##SinglyListIteratorFirst(singly_list_type_name##SinglyListHead* head) { \
        return head->first; \
    } \
    id_type singly_list_type_name##SinglyListIteratorNext(singly_list_type_name##SinglyListHead* head, id_type entry_id) { \
        singly_list_type_name##SinglyListEntry* entry = referencer##_Reference(head, entry_id); \
        id_type next_id = entry->next; \
        referencer##_Dereference(head, entry); \
        return next_id; \
    } \


CUTILS_CONTAINER_SINGLY_LIST_DECLARATION(, struct _SinglyListEntry*)



#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_SINGLY_LIST_H_