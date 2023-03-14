/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
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
    typedef singly_list_type_name##SinglyListEntry singly_list_type_name##SinglyListHead; \
    void singly_list_type_name##SinglyListHeadInit(id_type head_id); \
    bool singly_list_type_name##SinglyListIsEmpty(id_type head_id); \
    void singly_list_type_name##SinglyListPutFirst(id_type head_id, id_type entry_id); \
    id_type singly_list_type_name##SinglyListDeleteFirst(id_type head_id); \
    id_type singly_list_type_name##SinglyListDeleteEntry(id_type prev_id, id_type del_entry_id); \
    size_t singly_list_type_name##SinglyListGetCount(id_type head_id); \
    id_type singly_list_type_name##SinglyListFirst(id_type head_id); \
    id_type singly_list_type_name##SinglyListNext(id_type entry_id); \

#define CUTILS_CONTAINER_SINGLY_LIST_DEFINE(singly_list_type_name, id_type, referencer) \
    void singly_list_type_name##SinglyListHeadInit(id_type head_id) { \
        singly_list_type_name##SinglyListHead* head = referencer##_Reference(head_id); \
        head->next = referencer##_InvalidId; \
        referencer##_Dereference(head); \
    } \
    bool singly_list_type_name##SinglyListIsEmpty(id_type head_id) { \
        singly_list_type_name##SinglyListHead* head = referencer##_Reference(head_id); \
        bool is_empty = head->next == referencer##_InvalidId; \
        referencer##_Dereference(head); \
        return is_empty; \
    } \
    void singly_list_type_name##SinglyListPutFirst(id_type head_id, id_type entry_id) { \
        singly_list_type_name##SinglyListHead* head = referencer##_Reference(head_id); \
        singly_list_type_name##SinglyListEntry* entry = referencer##_Reference(entry_id); \
        id_type old_id = head->next; \
        singly_list_type_name##SinglyListEntry* old = referencer##_Reference(old_id); \
        head->next = entry_id; \
        entry->next = old_id; \
        referencer##_Dereference(head); \
        referencer##_Dereference(entry); \
        referencer##_Dereference(old); \
    } \
    id_type singly_list_type_name##SinglyListDeleteFirst(id_type head_id) { \
        singly_list_type_name##SinglyListHead* head = referencer##_Reference(head_id); \
        id_type old_id = head->next; \
        singly_list_type_name##SinglyListEntry* old = referencer##_Reference(old_id); \
        head->next = old->next; \
        referencer##_Dereference(head); \
        referencer##_Dereference(old); \
        return old_id; \
    } \
    id_type singly_list_type_name##SinglyListDeleteEntry(id_type prev_id, id_type del_entry_id) { \
        singly_list_type_name##SinglyListEntry* prev = referencer##_Reference(prev_id); \
        singly_list_type_name##SinglyListEntry* del_entry = referencer##_Reference(del_entry_id); \
        prev->next = del_entry->next; \
        referencer##_Dereference(prev); \
        referencer##_Dereference(del_entry); \
        return del_entry_id; \
    } \
    size_t singly_list_type_name##SinglyListGetCount(id_type head_id) { \
        size_t count = 0; \
        singly_list_type_name##SinglyListHead* head = referencer##_Reference(head_id); \
        id_type cur_id = head->next; \
        while (cur_id != referencer##_InvalidId) { \
            count++; \
            singly_list_type_name##SinglyListEntry* cur = referencer##_Reference(cur_id); \
            cur_id = cur->next; \
            referencer##_Dereference(cur); \
        } \
        return count; \
    } \
    id_type singly_list_type_name##SinglyListFirst(id_type head_id) { \
        singly_list_type_name##SinglyListHead* head = referencer##_Reference(head_id); \
        id_type next_id = head->next; \
        referencer##_Dereference(head); \
        return next_id; \
    } \
    id_type singly_list_type_name##SinglyListNext(id_type entry_id) { \
        singly_list_type_name##SinglyListEntry* entry = referencer##_Reference(entry_id); \
        id_type next_id = entry->next; \
        referencer##_Dereference(entry); \
        return next_id; \
    } \


CUTILS_CONTAINER_SINGLY_LIST_DECLARATION(, struct _SinglyListEntry*)




#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_SINGLY_LIST_H_