/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_LRU_LIST_H_
#define LIBYUC_CONTAINER_LRU_LIST_H_

#include <libyuc/basic.h>

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
LruListEntry* LruListGetTail(LruList* list);
LruListEntry* LruListPut(LruList* list, LruListEntry* entry);
bool LruListDeleteByKey(LruList* list, LIBYUC_CONTAINER_LRU_LIST_Type_Key* key);
void LruListDeleteByEntry(LruList* list, LruListEntry* entry);

#include <libyuc/container/lru_list.c>


#endif // LIBYUC_CONTAINER_LRU_LIST_H_