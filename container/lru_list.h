#ifndef CUTILS_CONTAINER_LRU_H_
#define CUTILS_CONTAINER_LRU_H_

#include <CUtils/object.h>
#include <CUtils/container/list.h>
#include <CUtils/container/hash_table.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CUTILS_CONTAINER_LRU_LIST_DECLARATION(lru_type_name, key_type) \
	typedef struct _##lru_type_name##LruListEntry { \
		ListEntry list_entry; \
	} lru_type_name##LruListEntry; \
	typedef struct _##lru_type_name##LruHashEntry { \
		lru_type_name##LruListEntry* lru_entry; \
	} lru_type_name##LruHashEntry; \
	CUTILS_CONTAINER_HASH_TABLE_DECLARATION(lru_type_name##LruList, lru_type_name##LruHashEntry, key_type) \
	typedef struct _LruList { \
		lru_type_name##LruListHashTable hash_table; \
		ListHead list_head; \
		size_t max_count; \
	} lru_type_name##LruList; \
	void lru_type_name##LruListInit(lru_type_name##LruList* list, size_t max_count); \
	lru_type_name##LruListEntry* lru_type_name##LruListGet(lru_type_name##LruList* list, key_type* key, bool put_first); \
	lru_type_name##LruListEntry* lru_type_name##LruListPut(lru_type_name##LruList* list, lru_type_name##LruListEntry* entry); \
	lru_type_name##LruListEntry* lru_type_name##LruListPop(lru_type_name##LruList* list); \
	lru_type_name##LruListEntry* lru_type_name##LruListDelete(lru_type_name##LruList* list, key_type* key); \
	
#define CUTILS_CONTAINER_LRU_LIST_DEFINE(lru_type_name, key_type, accessor, allocater, hasher, comparer) \
	forceinline key_type lru_type_name##LruHashEntryAccessor_GetKey(lru_type_name##LruListHashTable* table, lru_type_name##LruHashEntry hash_entry){ \
		return accessor##_GetKey((lru_type_name##LruList*)table, *(hash_entry.lru_entry)); \
	} \
    CUTILS_CONTAINER_HASH_TABLE_DEFINE(lru_type_name##LruList, lru_type_name##LruHashEntry, key_type, allocater, lru_type_name##LruHashEntryAccessor, CUTILS_OBJECT_MOVER_DEFALUT, hasher, comparer) \
    \
	void lru_type_name##LruListInit(lru_type_name##LruList* list, size_t max_count) { \
		lru_type_name##LruListHashTableInit(&list->hash_table, max_count, 0); \
		ListInit(&list->list_head); \
		list->max_count = max_count; \
	} \
	lru_type_name##LruListEntry* lru_type_name##LruListGet(lru_type_name##LruList* list, key_type* key, bool put_first) { \
		lru_type_name##LruHashEntry* hash_entry = lru_type_name##LruListHashTableFind(&list->hash_table, key); \
		if (!hash_entry) { \
			return NULL; \
		} \
		if (put_first) { \
			ListPutFirst(&list->list_head, ListDeleteEntry(&list->list_head, &hash_entry->lru_entry->list_entry)); \
		} \
		return &hash_entry->lru_entry; \
	} \
	lru_type_name##LruListEntry* lru_type_name##LruListPut(lru_type_name##LruList* list, lru_type_name##LruListEntry* entry) { \
		key_type key = accessor##_GetKey(list, entry); \
		lru_type_name##LruHashEntry* hash_entry = lru_type_name##LruListHashTableFind(&list->hash_table, &key); \
		if (hash_entry) { \
			lru_type_name##LruListDelete(list, &key); \
		} \
		else if (lru_type_name##LruListHashTableGetCount(&list->hash_table) >= list->max_count) { \
			hash_entry = lru_type_name##LruListPop(list); \
		} \
		lru_type_name##LruHashEntry put_hash_entry; \
		put_hash_entry.lru_entry = entry; \
		lru_type_name##LruListHashTablePut(&list->hash_table, &put_hash_entry); \
		ListPutFirst(&list->list_head, &entry->list_entry); \
		return hash_entry; \
	} \
	lru_type_name##LruListEntry* lru_type_name##LruListPop(lru_type_name##LruList* list) { \
		ListEntry* del_list_entry = ListDeleteLast(&list->list_head); \
		key_type key = accessor##_GetKey(list, *(lru_type_name##LruListEntry*)del_list_entry); \
		lru_type_name##LruListHashTableDelete(&list->hash_table, &key); \
		return (lru_type_name##LruListEntry*)del_list_entry; \
	} \
	lru_type_name##LruListEntry* lru_type_name##LruListDelete(lru_type_name##LruList* list, key_type* key) { \
		lru_type_name##LruHashEntry* del_hash_entry = lru_type_name##HashTableDelete(&list->hash_table, key); \
		if (del_hash_entry) { \
			ListDeleteEntry(&list->list_head, &del_hash_entry->lru_entry->list_entry); \
		} \
		return del_hash_entry; \
	} \


CUTILS_CONTAINER_LRU_LIST_DECLARATION(Int, int)
typedef struct _IntLru_Entry {
	IntLruListEntry lru_entry;
	int key;
} IntLru_Entry;
#define IntLruListEntryAccessor_GetKey(LRU_LIST, LRU_LIST_ENTRY) (((IntLru_Entry*)&(LRU_LIST_ENTRY))->key)
#define IntLruListEntryAccessor IntLruListEntryAccessor

//CUTILS_CONTAINER_LRU_LIST_DEFINE(Int, int, IntLruListEntryAccessor, CUTILS_OBJECT_ALLOCATOR_DEFALUT, CUTILS_OBJECT_HASHER_DEFALUT, CUTILS_OBJECT_COMPARER_DEFALUT)


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_LRU_H_