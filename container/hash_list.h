/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_HASH_LIST_H_
#define LIBYUC_CONTAINER_HASH_LIST_H_

#include <libyuc/object.h>
#include <libyuc/container/list.h>
#include <libyuc/container/hash_table.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIBYUC_CONTAINER_HASH_LIST_DECLARATION(hash_list_type_name, key_type) \
  typedef struct _##hash_list_type_name##HashListEntry { \
    ListEntry list_entry; \
  } hash_list_type_name##HashListEntry; \
  typedef struct _##hash_list_type_name##HashListHashEntry { \
    hash_list_type_name##HashListEntry* hash_list_entry; \
  } hash_list_type_name##HashListHashEntry; \
  LIBYUC_CONTAINER_HASH_TABLE_DECLARATION(hash_list_type_name##HashList, hash_list_type_name##HashListHashEntry, key_type) \
  typedef struct _HashList { \
    hash_list_type_name##HashListHashTable hash_table; \
    key_type empty_key; \
    ListHead list_head; \
    size_t max_count; \
  } hash_list_type_name##HashList; \
  void hash_list_type_name##HashListInit(hash_list_type_name##HashList* list, size_t max_count, const key_type* empty_key); \
  hash_list_type_name##HashListEntry* hash_list_type_name##HashListGet(hash_list_type_name##HashList* list, key_type* key, bool put_first); \
  hash_list_type_name##HashListEntry* hash_list_type_name##HashListPut(hash_list_type_name##HashList* list, hash_list_type_name##HashListEntry* entry); \
  hash_list_type_name##HashListEntry* hash_list_type_name##HashListPop(hash_list_type_name##HashList* list); \
  hash_list_type_name##HashListEntry* hash_list_type_name##HashListDelete(hash_list_type_name##HashList* list, key_type* key); \
  hash_list_type_name##HashListEntry* hash_list_type_name##HashListIteratorLast(hash_list_type_name##HashList* list); \


#define LIBYUC_CONTAINER_HASH_LIST_DEFINE(hash_list_type_name, key_type, accessor, allocater, hasher, comparer) \
  forceinline key_type* hash_list_type_name##HashListHashEntryAccessor_GetKey(hash_list_type_name##HashListHashTable* table, hash_list_type_name##HashListHashEntry* hash_entry) { \
    if (!hash_entry->hash_list_entry) return &ObjectGetFromField(table, hash_list_type_name##HashList, hash_table)->empty_key; \
    return accessor##_GetKey(ObjectGetFromField(table, hash_list_type_name##HashList, hash_table), hash_entry->hash_list_entry); \
  } \
  forceinline void hash_list_type_name##HashListHashEntryAccessor_SetKey(hash_list_type_name##HashListHashTable* table, hash_list_type_name##HashListHashEntry* hash_entry, const key_type* key) { \
    accessor##_SetKey(ObjectGetFromField(table, hash_list_type_name##HashList, hash_table), hash_entry->hash_list_entry, key); \
  } \
  LIBYUC_CONTAINER_HASH_TABLE_DEFINE(hash_list_type_name##HashList, hash_list_type_name##HashListHashEntry, key_type, allocater, hash_list_type_name##HashListHashEntryAccessor, LIBYUC_OBJECT_MOVER_DEFALUT, hasher, comparer) \
  \
  void hash_list_type_name##HashListInit(hash_list_type_name##HashList* list, size_t max_count, const key_type* empty_key) { \
    list->empty_key = *empty_key; \
    hash_list_type_name##HashListHashEntry empty_hash_entry; \
    empty_hash_entry.hash_list_entry = NULL; \
    hash_list_type_name##HashListHashTableInit(&list->hash_table, max_count, 0, &empty_hash_entry); \
    ListInit(&list->list_head); \
    list->max_count = max_count; \
  } \
  hash_list_type_name##HashListEntry* hash_list_type_name##HashListGet(hash_list_type_name##HashList* list, key_type* key, bool put_first) { \
    hash_list_type_name##HashListHashEntry* hash_entry = hash_list_type_name##HashListHashTableFind(&list->hash_table, key); \
    if (!hash_entry) { \
      return NULL; \
    } \
    if (put_first) { \
      ListPutFirst(&list->list_head, ListDeleteEntry(&list->list_head, &hash_entry->hash_list_entry->list_entry)); \
    } \
    return hash_entry->hash_list_entry; \
  } \
  hash_list_type_name##HashListEntry* hash_list_type_name##HashListPut(hash_list_type_name##HashList* list, hash_list_type_name##HashListEntry* entry) { \
    key_type* key = accessor##_GetKey(list, entry); \
    hash_list_type_name##HashListHashEntry put_hash_entry; \
    put_hash_entry.hash_list_entry = entry; \
    hash_list_type_name##HashListHashTableIterator iter; \
    hash_list_type_name##HashListHashEntry* hash_entry = hash_list_type_name##HashListHashTableIteratorLocate(&list->hash_table, &iter, key); \
    if (hash_entry) { \
      hash_list_type_name##HashListDelete(list, key); \
    } \
    hash_list_type_name##HashListHashTableIteratorPut(&list->hash_table, &iter, &put_hash_entry); \
    ListPutFirst(&list->list_head, &entry->list_entry); \
    return hash_entry ? hash_entry->hash_list_entry : NULL; \
  } \
  hash_list_type_name##HashListEntry* hash_list_type_name##HashListPop(hash_list_type_name##HashList* list) { \
    ListEntry* del_list_entry = ListDeleteLast(&list->list_head); \
    if (!del_list_entry) return NULL; \
    key_type* key = accessor##_GetKey(list, (hash_list_type_name##HashListEntry*)del_list_entry); \
    hash_list_type_name##HashListHashTableDelete(&list->hash_table, key); \
    return (hash_list_type_name##HashListEntry*)del_list_entry; \
  } \
  hash_list_type_name##HashListEntry* hash_list_type_name##HashListDelete(hash_list_type_name##HashList* list, key_type* key) { \
    hash_list_type_name##HashListEntry* hash_list_entry = NULL; \
    hash_list_type_name##HashListHashTableIterator iter; \
    hash_list_type_name##HashListHashEntry* del_hash_entry = hash_list_type_name##HashListHashTableIteratorLocate(&list->hash_table, &iter, key); \
    if (del_hash_entry) { \
      hash_list_entry = del_hash_entry->hash_list_entry; \
      ListDeleteEntry(&list->list_head, &del_hash_entry->hash_list_entry->list_entry); \
    } \
    hash_list_type_name##HashListHashTableIteratorDelete(&list->hash_table, &iter); \
    return del_hash_entry ? hash_list_entry : NULL; \
  } \
  hash_list_type_name##HashListEntry* hash_list_type_name##HashListIteratorLast(hash_list_type_name##HashList* list) { \
    ListEntry* list_entry = ListLast(&list->list_head); \
    return (hash_list_type_name##HashListEntry*)list_entry; \
  } \


//LIBYUC_CONTAINER_LRU_LIST_DECLARATION(Int, int)
//typedef struct _IntLru_Entry {
//  IntHashListEntry hash_list_entry;
//  int key;
//} IntLru_Entry;
//#define IntHashListEntryAccessor_GetKey(LRU_LIST, LRU_LIST_ENTRY) (((IntLru_Entry*)(LRU_LIST_ENTRY))->key)
//#define IntHashListEntryAccessor IntHashListEntryAccessor

//LIBYUC_CONTAINER_LRU_LIST_DEFINE(Int, int, IntHashListEntryAccessor, LIBYUC_OBJECT_ALLOCATOR_DEFALUT, LIBYUC_OBJECT_HASHER_DEFALUT, LIBYUC_OBJECT_COMPARER_DEFALUT)


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_HASH_LIST_H_