#ifndef CUTILS_CONTAINER_LRU_H_
#define CUTILS_CONTAINER_LRU_H_

#include <CUtils/object.h>
#include <CUtils/container/list.h>
#include <CUtils/container/hash_table.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CUTILS_CONTAINER_LRU_LIST_DECLARATION(lru_type_name, element_type, key_type) \
    CUTILS_CONTAINER_HASH_TABLE_DECLARATION(lru_type_name##LruList, element_type, key_type) \
    \
	typedef struct _##lru_type_name##LruEntry { \
		ListEntry list_entry; \
	} lru_type_name##LruEntry; \
	typedef struct _LruList { \
		lru_type_name##LruListHashTable hash_table; \
		ListHead list_head; \
	} lru_type_name##LruList; \

	

#define CUTILS_CONTAINER_LRU_LIST_DEFINE(lru_type_name, element_type, key_type, allocater, accessor, obj_mover, hasher, comparer) \
	CUTILS_CONTAINER_HASH_TABLE_DEFINE(lru_type_name##LruList, element_type, key_type) \
    \
	void lru_type_name##LruListInit(lru_type_name##LruList* list, size_t max_count) { \
		lru_type_name##HashTableInit(&list->hash_table, max_count, 0); \
		ListHeadInit(&list->list_head); \
		list->lru_entry_field_offset = lru_entry_field_offset; \
	} \
	element_type* lru_type_name##LruListGet(lru_type_name##LruList* list, key_type* key, bool put_first) { \
		element_type* obj = lru_type_name##HashTableFind(&list->hash_table, key); \
		if (!obj) { \
			return NULL; \
		} \
		if (put_first) { \
			ListPutFirst(&list->list_head, ListDeleteEntry(&list->list_head, &entry->list_entry)); \
		} \
		return obj; \
	} \
	element_type* lru_type_name##LruListPut(lru_type_name##LruList* list, lru_type_name##LruEntry* entry) { \
		void* obj = ObjectGetFromFieldOffset(entry, list->lru_entry_field_offset, void); \
		void* key = ObjectGetFieldByOffset(obj, list->hash_table.keyFieldOffset, void); \
		void* del_obj = HashTableFind(&list->hash_table, key); \
		if (del_obj) { \
			LruListDelete(list, key); \
		} \
		else if (HashTableGetCount(&list->hash_table) >= HashTableGetCapacity(&list->hash_table)) { \
			del_obj = LruListPop(list); \
		} \
		HashTableInsert(&list->hash_table, obj);
		LruEntry* new_entry = ObjectGetFieldByOffset(obj, list->lru_entry_field_offset, LruEntry);
		ListInsertNext(&list->list_head, &new_entry->list_entry);
		return del_obj;
	}
	void* LruListPop(LruList* list) {
		ListEntry* del_list_entry = ListRemovePrev(&list->list_head);
		void* del_obj = ObjectGetFromFieldOffset(del_list_entry, list->lru_entry_field_offset, void);
		HashTableDelete(&list->hash_table, ObjectGetFieldByOffset(del_obj, list->hash_table.keyFieldOffset, void));
		LruEntry* del_entry = ObjectGetFieldByOffset(del_obj, list->lru_entry_field_offset, LruEntry);
		ListRemoveEntry(&del_entry->list_entry, true);
		return del_obj;
	}

	void* LruListDelete(LruList* list, void* key) {
		void* del_obj = HashTableDelete(&list->hash_table, key);
		if (del_obj) {
			LruEntry* del_entry = ObjectGetFieldByOffset(del_obj, list->lru_entry_field_offset, LruEntry);
			ListRemoveEntry(&del_entry->list_entry, true);
		}
		return del_obj;
	}

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_LRU_H_