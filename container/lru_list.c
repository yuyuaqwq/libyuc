#include <CUtils/container/lru_list.h>

void LruListInit(LruList* list, size_t capacity, int lru_entry_field_offset, int key_field_offset, int key_size, HashU32Func hash_func, CmpFunc cmp_func) {
	HashTableInit(&list->hash_table, capacity, 101, key_field_offset, key_size, hash_func, cmp_func);
	ListHeadInit(&list->list_head);
	list->lru_entry_field_offset = lru_entry_field_offset;
}

void* LruListGet(LruList* list, void* key) {
	void* obj = HashTableFind(&list->hash_table, key);
	if (!obj) {
		return NULL;
	}
	LruEntry* entry = GetFieldByFieldOffset(obj, list->lru_entry_field_offset, LruEntry);
	ListInsertNext(&list->list_head, ListRemoveEntry(&entry->list_entry, true));
	return obj;
}

void* LruListPut(LruList* list, void* obj) {
	void* key = GetFieldByFieldOffset(obj, list->hash_table.keyFieldOffset, void);
	void* del_obj = HashTableFind(&list->hash_table, key);
	if (del_obj) {
		HashTableDelete(&list->hash_table, key);
		LruEntry* del_entry = GetFieldByFieldOffset(del_obj, list->lru_entry_field_offset, LruEntry);
		ListRemoveEntry(&del_entry->list_entry, true);
	}
	else if (HashTableGetCount(&list->hash_table) >= HashTableGetCapacity(&list->hash_table)) {
		ListEntry* del_entry = ListRemovePrev(&list->list_head);
		del_obj = GetObjByFieldOffset(del_entry, list->lru_entry_field_offset, void);
		HashTableDelete(&list->hash_table, GetFieldByFieldOffset(del_obj, list->hash_table.keyFieldOffset, void));
	}
	HashTableInsert(&list->hash_table, obj);
	LruEntry* new_entry = GetFieldByFieldOffset(obj, list->lru_entry_field_offset, LruEntry);
	ListInsertNext(&list->list_head, &new_entry->list_entry);
	return del_obj;
}
