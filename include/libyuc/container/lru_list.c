#include <libyuc/basic.h>

#include <libyuc/container/lru_list.def>

#define LIBYUC_CONTAINER_LIST_CLASS_NAME MAKE_NAME(LIBYUC_CONTAINER_LRU_LIST_CLASS_NAME, LruLink)
#include <libyuc/container/list.c>


//forceinline LIBYUC_CONTAINER_LRU_LIST_Type_Key* LruListHashEntryAccessor_GetKey(LruHashTable* table, LruListHashEntry* hash_entry) {
//    if (!hash_entry->lru_list_entry) {
//        return NULL;
//    }
//    return LIBYUC_CONTAINER_LRU_LIST_ACCESSOR_GetKey(ObjectGetFromField(table, LruList, hash_table), hash_entry->lru_list_entry);
//}
//forceinline void LruListHashEntryAccessor_SetKey(LruHashTable* table, LruListHashEntry* hash_entry, const LIBYUC_CONTAINER_LRU_LIST_Type_Key* key) {
//    LIBYUC_CONTAINER_LRU_LIST_ACCESSOR_SetKey(ObjectGetFromField(table, LruList, hash_table), hash_entry->lru_list_entry, key);
//}
//forceinline ptrdiff_t LruListHashEntryComparer_Cmp(LruHashTable* table, LIBYUC_CONTAINER_LRU_LIST_Type_Key* key1, const LIBYUC_CONTAINER_LRU_LIST_Type_Key* key2) {
//    if (key1 == key2) {
//        return 0;
//    }
//    if (key1 == NULL || key2 == NULL) {
//        return 1;
//    }
//    return *key1 - *key2;
//}

#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME MAKE_NAME(LIBYUC_CONTAINER_LRU_LIST_CLASS_NAME, Lru)
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element LruListHashEntry
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key LIBYUC_CONTAINER_LRU_LIST_Type_Key
#define LIBYUC_CONTAINER_HASH_TABLE_COMPARER_Cmp(MAIN_OBJ, KEY1, KEY2) LIBYUC_CONTAINER_LRU_LIST_COMPARER_Cmp(MAIN_OBJ, KEY1, KEY2)
#include <libyuc/container/hash_table.c>


void LruListInit(LruList* list, size_t max_count) {
    LruHashTableInit(&list->hash_table, max_count, 0);
    LruLinkListInit(&list->list_head);
    list->max_count = max_count;
}

LruListEntry* LruListGet(LruList* list, LIBYUC_CONTAINER_LRU_LIST_Type_Key* key, bool put_first) {
    LruListHashEntry* hash_entry = LruHashTableFind(&list->hash_table, key);
    if (!hash_entry) {
        return NULL;
    }
    if (put_first) {
        LruLinkListPutFirst(&list->list_head, LruLinkListDeleteEntry(&list->list_head, &hash_entry->lru_list_entry->list_entry));
    }
    return hash_entry->lru_list_entry;
}

LruListEntry* LruListGetTail(LruList* list) {
    return (LruListEntry*)LruLinkListLast(&list->list_head);
}

LruListEntry* LruListPut(LruList* list, LruListEntry* entry) {
    LIBYUC_CONTAINER_LRU_LIST_Type_Key* key = LIBYUC_CONTAINER_LRU_LIST_ACCESSOR_GetKey(list, entry);
    LruListHashEntry put_hash_entry;
    put_hash_entry.lru_list_entry = entry;
    LruHashTableIterator iter;
    LruListHashEntry* hash_entry = LruHashTableIteratorLocate(&list->hash_table, &iter, key);
    if (hash_entry) {
        LruListDeleteByKey(list, key);
    }
    LruHashTablePut(&list->hash_table, &put_hash_entry);
    LruLinkListPutFirst(&list->list_head, &entry->list_entry);
    return hash_entry ? hash_entry->lru_list_entry : NULL;
}

bool LruListDeleteByKey(LruList* list, LIBYUC_CONTAINER_LRU_LIST_Type_Key* key) {
    LruHashTableIterator iter;
    LruListHashEntry* del_hash_entry = LruHashTableIteratorLocate(&list->hash_table, &iter, key);
    if (del_hash_entry) {
        LruLinkListDeleteEntry(&list->list_head, &del_hash_entry->lru_list_entry->list_entry);
    }
    LruHashTableIteratorDelete(&list->hash_table, &iter);
    return del_hash_entry ? true : false;
}

void LruListDeleteByEntry(LruList* list, LruListEntry* entry) {
    LIBYUC_CONTAINER_LRU_LIST_Type_Key* key = LIBYUC_CONTAINER_LRU_LIST_ACCESSOR_GetKey(list, entry);
    LruHashTableDelete(&list->hash_table, key);
}