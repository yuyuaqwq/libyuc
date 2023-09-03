/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_LRU_LIST_H_
#define LIBYUC_CONTAINER_LRU_LIST_H_

#include <libyuc/basic.h>
#include <libyuc/container/list.h>
#include <libyuc/container/hash_table.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIBYUC_CONTAINER_LRU_LIST_DECLARATION(lru_list_type_name, key_type) \
    typedef struct _##lru_list_type_name##LruListEntry { \
        ListEntry list_entry; \
    } lru_list_type_name##LruListEntry; \
    typedef struct _##lru_list_type_name##LruListHashEntry { \
        lru_list_type_name##LruListEntry* lru_list_entry; \
    } lru_list_type_name##LruListHashEntry; \
    LIBYUC_CONTAINER_HASH_TABLE_DECLARATION(lru_list_type_name##LruList, lru_list_type_name##LruListHashEntry, key_type) \
    typedef struct _HashList { \
        lru_list_type_name##LruListHashTable hash_table; \
        ListHead list_head; \
        size_t max_count; \
    } lru_list_type_name##LruList; \
    void lru_list_type_name##LruListInit(lru_list_type_name##LruList* list, size_t max_count); \
    lru_list_type_name##LruListEntry* lru_list_type_name##LruListGet(lru_list_type_name##LruList* list, key_type* key, bool put_first); \
    lru_list_type_name##LruListEntry* lru_list_type_name##LruListPut(lru_list_type_name##LruList* list, lru_list_type_name##LruListEntry* entry); \
    lru_list_type_name##LruListEntry* lru_list_type_name##LruListPop(lru_list_type_name##LruList* list); \
    lru_list_type_name##LruListEntry* lru_list_type_name##LruListDelete(lru_list_type_name##LruList* list, key_type* key); \
    lru_list_type_name##LruListEntry* lru_list_type_name##LruListIteratorLast(lru_list_type_name##LruList* list); \


#define LIBYUC_CONTAINER_LRU_LIST_DEFINE(lru_list_type_name, key_type, accessor, allocater, hasher, comparer) \
    forceinline key_type* lru_list_type_name##LruListHashEntryAccessor_GetKey(lru_list_type_name##LruListHashTable* table, lru_list_type_name##LruListHashEntry* hash_entry) { \
        if (!hash_entry->lru_list_entry) return &ObjectGetFromField(table, lru_list_type_name##LruList, hash_table)->empty_key; \
        return accessor##_GetKey(ObjectGetFromField(table, lru_list_type_name##LruList, hash_table), hash_entry->lru_list_entry); \
    } \
    forceinline void lru_list_type_name##LruListHashEntryAccessor_SetKey(lru_list_type_name##LruListHashTable* table, lru_list_type_name##LruListHashEntry* hash_entry, const key_type* key) { \
        accessor##_SetKey(ObjectGetFromField(table, lru_list_type_name##LruList, hash_table), hash_entry->lru_list_entry, key); \
    } \
    LIBYUC_CONTAINER_HASH_TABLE_DEFINE(lru_list_type_name##LruList, uint32_t, lru_list_type_name##LruListHashEntry, key_type, allocater, lru_list_type_name##LruListHashEntryAccessor, LIBYUC_BASIC_MOVER_DEFALUT, hasher, comparer) \
    \
    void lru_list_type_name##LruListInit(lru_list_type_name##LruList* list, size_t max_count) { \
        lru_list_type_name##LruListHashTableInit(&list->hash_table, max_count, 0); \
        ListInit(&list->list_head); \
        list->max_count = max_count; \
    } \
    lru_list_type_name##LruListEntry* lru_list_type_name##LruListGet(lru_list_type_name##LruList* list, key_type* key, bool put_first) { \
        lru_list_type_name##LruListHashEntry* hash_entry = lru_list_type_name##LruListHashTableFind(&list->hash_table, key); \
        if (!hash_entry) { \
            return NULL; \
        } \
        if (put_first) { \
            ListPutFirst(&list->list_head, ListDeleteEntry(&list->list_head, &hash_entry->lru_list_entry->list_entry)); \
        } \
        return hash_entry->lru_list_entry; \
    } \
    lru_list_type_name##LruListEntry* lru_list_type_name##LruListPut(lru_list_type_name##LruList* list, lru_list_type_name##LruListEntry* entry) { \
        key_type* key = accessor##_GetKey(list, entry); \
        lru_list_type_name##LruListHashEntry put_hash_entry; \
        put_hash_entry.lru_list_entry = entry; \
        lru_list_type_name##LruListHashTableIterator iter; \
        lru_list_type_name##LruListHashEntry* hash_entry = lru_list_type_name##LruListHashTableIteratorLocate(&list->hash_table, &iter, key); \
        if (hash_entry) { \
            lru_list_type_name##LruListDelete(list, key); \
        } \
        lru_list_type_name##LruListHashTableIteratorPut(&list->hash_table, &iter, &put_hash_entry); \
        ListPutFirst(&list->list_head, &entry->list_entry); \
        return hash_entry ? hash_entry->lru_list_entry : NULL; \
    } \
    lru_list_type_name##LruListEntry* lru_list_type_name##LruListPop(lru_list_type_name##LruList* list) { \
        ListEntry* del_list_entry = ListDeleteLast(&list->list_head); \
        if (!del_list_entry) return NULL; \
        key_type* key = accessor##_GetKey(list, (lru_list_type_name##LruListEntry*)del_list_entry); \
        lru_list_type_name##LruListHashTableDelete(&list->hash_table, key); \
        return (lru_list_type_name##LruListEntry*)del_list_entry; \
    } \
    lru_list_type_name##LruListEntry* lru_list_type_name##LruListDelete(lru_list_type_name##LruList* list, key_type* key) { \
        lru_list_type_name##LruListEntry* lru_list_entry = NULL; \
        lru_list_type_name##LruListHashTableIterator iter; \
        lru_list_type_name##LruListHashEntry* del_hash_entry = lru_list_type_name##LruListHashTableIteratorLocate(&list->hash_table, &iter, key); \
        if (del_hash_entry) { \
            lru_list_entry = del_hash_entry->lru_list_entry; \
            ListDeleteEntry(&list->list_head, &del_hash_entry->lru_list_entry->list_entry); \
        } \
        lru_list_type_name##LruListHashTableIteratorDelete(&list->hash_table, &iter); \
        return del_hash_entry ? lru_list_entry : NULL; \
    } \
    lru_list_type_name##LruListEntry* lru_list_type_name##LruListIteratorLast(lru_list_type_name##LruList* list) { \
        ListEntry* list_entry = ListLast(&list->list_head); \
        return (lru_list_type_name##LruListEntry*)list_entry; \
    } \


//LIBYUC_CONTAINER_LRU_LIST_DECLARATION(Int, int)
//typedef struct _IntLru_Entry {
//    IntHashListEntry lru_list_entry;
//    int key;
//} IntLru_Entry;
//#define IntHashListEntryAccessor_GetKey(LRU_LIST, LRU_LIST_ENTRY) (((IntLru_Entry*)(LRU_LIST_ENTRY))->key)
//#define IntHashListEntryAccessor IntHashListEntryAccessor

//LIBYUC_CONTAINER_LRU_LIST_DEFINE(Int, int, IntHashListEntryAccessor, LIBYUC_BASIC_ALLOCATOR_DEFALUT, LIBYUC_BASIC_HASHER_DEFALUT, LIBYUC_BASIC_COMPARER_DEFALUT)


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_LRU_LIST_H_