/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_LIST_H_
#define CUTILS_CONTAINER_LIST_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CUTILS_CONTAINER_LIST_DECLARATION(list_type_name, id_type) \
    typedef struct _##list_type_name##ListEntry { \
        id_type prev; \
        id_type next; \
    } list_type_name##ListEntry; \
    typedef struct _##list_type_name##ListHead { \
        id_type last; \
        id_type first; \
    } list_type_name##ListHead; \
    \
    void list_type_name##ListInit(list_type_name##ListHead* head); \
    void list_type_name##ListPutFirst(list_type_name##ListHead* head, id_type entry_id); \
    void list_type_name##ListPutLast(list_type_name##ListHead* head, id_type entry_id); \
    id_type list_type_name##ListDeleteEntry(list_type_name##ListHead* head, id_type entry_id); \
    id_type list_type_name##ListDeleteFirst(list_type_name##ListHead* head); \
    id_type list_type_name##ListDeleteLast(list_type_name##ListHead* head); \
    size_t list_type_name##ListGetCount(list_type_name##ListHead* head); \
    id_type list_type_name##ListFirst(list_type_name##ListHead* head); \
    id_type list_type_name##ListPrev(list_type_name##ListHead* head, id_type cur_id); \
    id_type list_type_name##ListNext(list_type_name##ListHead* head, id_type cur_id); \


#define CUTILS_CONTAINER_LIST_DEFINE(list_type_name, id_type, referencer) \
    void list_type_name##ListInit(list_type_name##ListHead* head) { \
        head->last = referencer##_InvalidId; \
        head->first = referencer##_InvalidId; \
    } \
    void list_type_name##ListPutFirst(list_type_name##ListHead* head, id_type entry_id) { \
        list_type_name##ListEntry* entry = referencer##_Reference(head, entry_id); \
        id_type old_first_id = head->first; \
        head->first = entry_id; \
        if (old_first_id == referencer##_InvalidId){ \
              assert(head->last == referencer##_InvalidId); \
            entry->prev = entry_id; \
            entry->next = entry_id; \
            head->last = entry_id; \
        } \
        else { \
            list_type_name##ListEntry* old_first = referencer##_Reference(head, old_first_id); \
            entry->prev = old_first->prev; \
            entry->next = old_first_id; \
            list_type_name##ListEntry* last = referencer##_Reference(head, old_first->prev); \
            old_first->prev = entry_id; \
            last->next = entry_id; \
            referencer##_Dereference(head, last); \
            referencer##_Dereference(head, old_first); \
        } \
        referencer##_Dereference(head, entry); \
    } \
    void list_type_name##ListPutLast(list_type_name##ListHead* head, id_type entry_id) { \
        list_type_name##ListEntry* entry = referencer##_Reference(head, entry_id); \
        id_type old_last_id = head->last; \
        head->last = entry_id; \
        if (old_last_id == referencer##_InvalidId){ \
              assert(head->first == referencer##_InvalidId); \
            entry->prev = entry_id; \
            entry->next = entry_id; \
            head->first = entry_id; \
        } \
        else { \
            list_type_name##ListEntry* old_last = referencer##_Reference(head, old_last_id); \
            entry->prev = old_last_id; \
            entry->next = old_last->next; \
            list_type_name##ListEntry* frist = referencer##_Reference(head, old_last->next); \
            old_last->next = entry_id; \
            frist->prev = entry_id; \
            referencer##_Dereference(head, first); \
            referencer##_Dereference(head, old_last); \
        } \
        referencer##_Dereference(head, entry); \
    } \
    id_type list_type_name##ListDeleteEntry(list_type_name##ListHead* head, id_type entry_id) { \
        if (head->first == head->last) { \
            entry_id = head->first; \
            list_type_name##ListInit(head); \
            return entry_id; \
        } \
        list_type_name##ListEntry* entry = referencer##_Reference(head, entry_id); \
        id_type prev_id = entry->prev; \
        id_type next_id = entry->next; \
        list_type_name##ListEntry* prev = referencer##_Reference(head, prev_id); \
        list_type_name##ListEntry* next = referencer##_Reference(head, next_id); \
        prev->next = next_id; \
        next->prev = prev_id; \
        referencer##_Dereference(head, prev); \
        referencer##_Dereference(head, next); \
        \
        if (entry_id == head->first) { \
            head->first = next_id; \
        } \
        else if (entry_id == head->last) { \
            head->last = prev_id; \
        } \
        referencer##_Dereference(head, entry); \
        return; \
    } \
    id_type list_type_name##ListDeleteFirst(list_type_name##ListHead* head) { \
        return list_type_name##ListDeleteEntry(head, head->first); \
    } \
    id_type list_type_name##ListDeleteLast(list_type_name##ListHead* head) { \
        return list_type_name##ListDeleteEntry(head, head->last); \
    } \
    size_t list_type_name##ListGetCount(list_type_name##ListHead* head) { \
        size_t count = 0; \
        id_type cur_id = list_type_name##ListFirst(head); \
        while (cur_id != referencer##_InvalidId) { \
            count++; \
            list_type_name##ListEntry* cur = referencer##_Reference(head, cur_id); \
            cur_id = list_type_name##ListNext(head, cur_id); \
            referencer##_Dereference(head, cur);\
        } \
        return count; \
    } \
    id_type list_type_name##ListFirst(list_type_name##ListHead* head) { \
        return head->first; \
    } \
    id_type list_type_name##ListPrev(list_type_name##ListHead* head, id_type cur_id) { \
        list_type_name##ListEntry* cur = referencer##_Reference(head, cur_id); \
        id_type prev_id = cur->prev; \
        referencer##_Dereference(head, cur); \
        if (prev_id == head->last) { \
            return referencer##_InvalidId; \
        } \
        return prev_id; \
    } \
    id_type list_type_name##ListNext(list_type_name##ListHead* head, id_type cur_id) { \
        list_type_name##ListEntry* cur = referencer##_Reference(head, cur_id); \
        id_type next_id = cur->next; \
        referencer##_Dereference(head, cur); \
        if (next_id == head->first) { \
            return referencer##_InvalidId; \
        } \
        return next_id; \
    } \


CUTILS_CONTAINER_LIST_DECLARATION(, struct _ListEntry*)



#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_LIST_H_