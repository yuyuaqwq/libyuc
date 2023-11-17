/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>

#include <libyuc/container/lru_list.def>

#define LIBYUC_CONTAINER_LIST_CLASS_NAME MAKE_NAME(LIBYUC_CONTAINER_LRU_LIST_CLASS_NAME, LruLink)
#include <libyuc/container/list.h>


typedef struct LruListEntry {
    LruLinkListEntry list_entry;
} LruListEntry;

typedef struct LruListHashEntry {
    LruListEntry* lru_entry;
} LruListHashEntry;


#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME MAKE_NAME(LIBYUC_CONTAINER_LRU_LIST_CLASS_NAME, Lru)
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element LruListHashEntry
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key LIBYUC_CONTAINER_LRU_LIST_Type_Key
#include <libyuc/container/hash_table.h>

typedef struct LruList {
    LruHashTable hash_table;
    LruLinkList list;
    size_t max_count;
} LruList;

void LruListInit(LruList* list, size_t max_count);
LruListEntry* LruListGetByKey(LruList* list, LIBYUC_CONTAINER_LRU_LIST_Type_Key* key, bool put_first);
LruListEntry* LruListGetTail(LruList* list);
LruListEntry* LruListPut(LruList* list, LruListEntry* entry);
void LruListDelete(LruList* list, LruListEntry* entry);
bool LruListDeleteByKey(LruList* list, LIBYUC_CONTAINER_LRU_LIST_Type_Key* key);

#include <libyuc/container/lru_list.undef>