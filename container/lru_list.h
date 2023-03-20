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
	} LruList; \



void LruListInit(LruList* list, size_t capacity, int lru_entry_field_offset, int obj_size, int key_field_offset, int key_size, HashU32Func hash_func, CmpFunc cmp_func);
#define LruListInitByField(list, capacity, objName, entryFieldName, keyFieldName) LruListInit((list), (capacity), ObjectGetFieldOffset(objName, entryFieldName), sizeof(objName),  ObjectGetFieldOffset(objName, keyFieldName), ObjectGetFieldSize(objName, keyFieldName), NULL, NULL)
void* LruListGet(LruList* list, void* key, bool put_first);
void* LruListPut(LruList* list, LruEntry* entry);
void* LruListPop(LruList* list);
void* LruListDelete(LruList* list, void* key);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_LRU_H_