#include <CUtils/container/lru_list.h>


void LruListInit(LruList* list, size_t capacity, int lru_entry_field_offset, int obj_size, int key_field_offset, int key_size, HashU32Func hash_func, CmpFunc cmp_func) {
	HashTableInit(&list->hash_table, capacity, 101, obj_size, key_field_offset, key_size, hash_func, cmp_func);
	ListHeadInit(&list->list_head);
	list->lru_entry_field_offset = lru_entry_field_offset;
}

void* LruListGet(LruList* list, void* key, bool put_first) {
	void* obj = HashTableFind(&list->hash_table, key);
	if (!obj) {
		return NULL;
	}
	if (put_first) {
		LruEntry* entry = ObjectGetFieldByOffset(obj, list->lru_entry_field_offset, LruEntry);
		ListInsertNext(&list->list_head, ListRemoveEntry(&entry->list_entry, true));
	}
	return obj;
}

void* LruListPut(LruList* list, LruEntry* entry) {
	void* obj = ObjectGetFromFieldOffset(entry, list->lru_entry_field_offset, void);
	void* key = ObjectGetFieldByOffset(obj, list->hash_table.keyFieldOffset, void);
	void* del_obj = HashTableFind(&list->hash_table, key);
	if (del_obj) {
		LruListDelete(list, key);
	}
	else if (HashTableGetCount(&list->hash_table) >= HashTableGetCapacity(&list->hash_table)) {
		del_obj = LruListPop(list);
	}
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