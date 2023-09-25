/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_LRU_LIST_H_
#define LIBYUC_CONTAINER_LRU_LIST_H_

#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <libyuc/container/lru_list.def>

#define LIBYUC_CONTAINER_LIST_CLASS_NAME LruLink
#include <libyuc/container/list.h>


typedef struct LruListEntry {
    LruLinkListEntry list_entry;
} LruListEntry;
typedef struct LruListHashEntry {
    LruListEntry* lru_list_entry;
} LruListHashEntry;

#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME LruList
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element LruListHashEntry
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key LIBYUC_CONTAINER_LRU_LIST_Type_Key
#include <libyuc/container/hash_table.h>

typedef struct HashList {
    LruListHashTable hash_table;
    LruLinkListHead list_head;
    size_t max_count;
} LruList;
void LruListInit(LruList* list, size_t max_count);
LruListEntry* LruListGet(LruList* list, LIBYUC_CONTAINER_LRU_LIST_Type_Key* key, bool put_first);
LruListEntry* LruListPut(LruList* list, LruListEntry* entry);
LruListEntry* LruListPop(LruList* list);
LruListEntry* LruListDelete(LruList* list, LIBYUC_CONTAINER_LRU_LIST_Type_Key* key);
LruListEntry* LruListIteratorLast(LruList* list);



forceinline LIBYUC_CONTAINER_LRU_LIST_Type_Key* LruListHashEntryAccessor_GetKey(LruListHashTable* table, LruListHashEntry* hash_entry) {
    if (!hash_entry->lru_list_entry) return &ObjectGetFromField(table, LruList, hash_table)->empty_key;
    return LIBYUC_CONTAINER_LRU_LIST_ACCESSOR_GetKey(ObjectGetFromField(table, LruList, hash_table), hash_entry->lru_list_entry);
}
forceinline void LruListHashEntryAccessor_SetKey(LruListHashTable* table, LruListHashEntry* hash_entry, const LIBYUC_CONTAINER_LRU_LIST_Type_Key* key) {
    LIBYUC_CONTAINER_LRU_LIST_ACCESSOR_SetKey(ObjectGetFromField(table, LruList, hash_table), hash_entry->lru_list_entry, key);
}
#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME LruList
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element LruListHashEntry
#include <libyuc/container/hash_table.c>

void LruListInit(LruList* list, size_t max_count) {
    LruListHashTableInit(&list->hash_table, max_count, 0);
    LruLinkListInit(&list->list_head);
    list->max_count = max_count;
}
LruListEntry* LruListGet(LruList* list, LIBYUC_CONTAINER_LRU_LIST_Type_Key* key, bool put_first) {
    LruListHashEntry* hash_entry = LruListHashTableFind(&list->hash_table, key);
    if (!hash_entry) {
        return NULL;
    }
    if (put_first) {
        LruLinkListPutFirst(&list->list_head, LruLinkListDeleteEntry(&list->list_head, &hash_entry->lru_list_entry->list_entry));
    }
    return hash_entry->lru_list_entry;
}
LruListEntry* LruListPut(LruList* list, LruListEntry* entry) {
    LIBYUC_CONTAINER_LRU_LIST_Type_Key* key = LIBYUC_CONTAINER_LRU_LIST_ACCESSOR_GetKey(list, entry);
    LruListHashEntry put_hash_entry;
    put_hash_entry.lru_list_entry = entry;
    LruListHashTableIterator iter;
    LruListHashEntry* hash_entry = LruListHashTableIteratorLocate(&list->hash_table, &iter, key);
    if (hash_entry) {
        LruListDelete(list, key);
    }
    LruListHashTablePut(&list->hash_table, &put_hash_entry);
    LruLinkListPutFirst(&list->list_head, &entry->list_entry);
    return hash_entry ? hash_entry->lru_list_entry : NULL;
}
LruListEntry* LruListPop(LruList* list) {
    LruLinkListEntry* del_list_entry = LruLinkListDeleteLast(&list->list_head);
    if (!del_list_entry) return NULL;
    LIBYUC_CONTAINER_LRU_LIST_Type_Key* key = LIBYUC_CONTAINER_LRU_LIST_ACCESSOR_GetKey(list, (LruListEntry*)del_list_entry);
    LruListHashTableDelete(&list->hash_table, key);
    return (LruListEntry*)del_list_entry;
}
LruListEntry* LruListDelete(LruList* list, LIBYUC_CONTAINER_LRU_LIST_Type_Key* key) {
    LruListEntry* lru_list_entry = NULL;
    LruListHashTableIterator iter;
    LruListHashEntry* del_hash_entry = LruListHashTableIteratorLocate(&list->hash_table, &iter, key);
    if (del_hash_entry) {
        lru_list_entry = del_hash_entry->lru_list_entry;
        LruLinkListDeleteEntry(&list->list_head, &del_hash_entry->lru_list_entry->list_entry);
    }
    LruListHashTableIteratorDelete(&list->hash_table, &iter);
    return del_hash_entry ? lru_list_entry : NULL;
}
LruListEntry* LruListIteratorLast(LruList* list) {
    LruLinkListEntry* list_entry = LruLinkListLast(&list->list_head);
    return (LruListEntry*)list_entry;
}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_LRU_LIST_H_