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
    typedef list_type_name##ListEntry list_type_name##ListHead; \

#define CUTILS_CONTAINER_LIST_DEFINE(list_type_name, id_type, referencer) \
    void list_type_name##ListHeadInit(id_type head_id) { \
        list_type_name##ListEntry* head = referencer##_Reference(head_id); \
        head->prev = head_id; \
        head->next = head_id; \
        referencer##_Dereference(head); \
    } \
    void list_type_name##ListPutFirst(id_type head_id, id_type entry_id) { \
        list_type_name##ListHead* head = referencer##_Reference(head_id); \
        list_type_name##ListEntry* entry = referencer##_Reference(entry_id); \
        id_type old_id = head->next; \
        list_type_name##ListEntry* old = referencer##_Reference(old_id); \
        head->next = entry_id; \
        entry->prev = head_id; \
        entry->next = old_id; \
        old->prev = entry_id; \
        referencer##_Dereference(head); \
        referencer##_Dereference(entry); \
        referencer##_Dereference(old); \
    } \
    void list_type_name##ListPutLast(id_type head_id, id_type entry_id) { \
        list_type_name##ListHead* head = referencer##_Reference(head_id); \
        list_type_name##ListEntry* entry = referencer##_Reference(entry_id); \
        id_type old_id = head->next; \
        list_type_name##ListEntry* old = referencer##_Reference(head->prev); \
        head->prev = entry_id; \
        entry->next = head_id; \
        entry->prev = old_id; \
        old->next = entry_id; \
        referencer##_Dereference(head); \
        referencer##_Dereference(entry); \
        referencer##_Dereference(old); \
    } \
    id_type list_type_name##ListDeleteEntry(id_type entry_id) { \
        list_type_name##ListEntry* entry = referencer##_Reference(entry_id); \
        id_type prev_id = entry->prev; \
        id_type next_id = entry->next; \
        list_type_name##ListEntry* prev = referencer##_Reference(prev_id); \
        list_type_name##ListEntry* next = referencer##_Reference(next_id); \
        prev->next = next_id; \
        next->prev = prev_id; \
        referencer##_Dereference(entry); \
        referencer##_Dereference(prev); \
        referencer##_Dereference(next); \
        return entry_id; \
    } \
    id_type list_type_name##ListDeleteFirst(id_type head_id) { \
        list_type_name##ListHead* head = referencer##_Reference(head_id); \
        id_type del_id = list_type_name##ListDeleteEntry(head->next, true); \
        referencer##_Dereference(head); \
        return del_id;\
    } \
    id_type list_type_name##ListDeleteLast(id_type head_id) { \
        list_type_name##ListHead* head = referencer##_Reference(head_id); \
        id_type del_id = list_type_name##ListDeleteEntry(head->prev, true); \
        referencer##_Dereference(head); \
        return del_id; \
    } \
    size_t list_type_name##ListGetCount(id_type head_id) { \
        size_t count = 0; \
        list_type_name##ListHead* head = referencer##_Reference(head_id); \
        id_type cur_id = head->next; \
        while (cur_id != head_id) { \
            count++; \
            list_type_name##ListHead* cur = referencer##_Reference(cur_id); \
            cur_id = cur->next; \
            referencer##_Dereference(cur);\
        } \
        referencer##_Dereference(head); \
        return count; \
    } \
    id_type list_type_name##ListFirst(id_type head_id) { \
        list_type_name##ListHead* head = referencer##_Reference(head_id); \
        id_type next_id = head->next; \
        referencer##_Dereference(head); \
        if (next_id == head_id) { \
            return referencer##_InvalidId; \
        } \
        return next_id; \
    } \
    id_type list_type_name##ListPrev(id_type head_id, id_type cur_id) { \
        list_type_name##ListHead* cur = referencer##_Reference(cur_id); \
        id_type prev_id = cur->prev; \
        referencer##_Dereference(cur); \
        if (prev_id == head_id) { \
            return referencer##_InvalidId; \
        } \
        return prev_id; \
    } \
    id_type list_type_name##ListNext(id_type head_id, id_type cur_id) { \
        list_type_name##ListHead* cur = referencer##_Reference(cur_id); \
        id_type next_id = cur->next; \
        referencer##_Dereference(cur); \
        if (next_id == head_id) { \
            return referencer##_InvalidId; \
        } \
        return next_id; \
    } \


CUTILS_CONTAINER_LIST_DECLARATION(, struct _ListEntry*)


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_LIST_H_