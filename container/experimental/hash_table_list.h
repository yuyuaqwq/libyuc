/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_HASH_TABLE_H_
#define LIBYUC_CONTAINER_HASH_TABLE_H_

#include <libyuc/basic.h>
#include <libyuc/algorithm/hash_code.h>
#include <libyuc/container/vector.h>
#include <libyuc/container/static_list.h>
#include <libyuc/container/singly_list.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 基于链地址法的哈希表
*/
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE 16
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR 75//%
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR 2

#define LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS
#ifdef LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS
#define LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DECLARATION \
    uint32_t obj_count; \
    uint32_t free_count; \
    uint32_t list_entry_count; \
    uint32_t list_head_count; \
    uint32_t max_list_count; \
    uint32_t cur_list_count; \

#define LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_1 \
    iter->obj_count = 0; \
    iter->free_count = 0; \
    iter->list_entry_count = 0; \
    iter->list_head_count = 0; \
    iter->max_list_count = 0; \
    iter->cur_list_count = 0; \

#define LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_2 \
    iter->cur_list_count++; \
    if (iter->cur_list_count > iter->max_list_count) { \
        iter->max_list_count = iter->cur_list_count; \
    } \
    iter->list_entry_count++; \

#define LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_3 \
    iter->free_count++;

#define LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_4 \
    iter->obj_count++; \

#define LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_5 \
    iter->cur_list_count = 0; \
    iter->list_head_count++; \

#else
#define LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DECLARATION 
#define LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_1
#define LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_2
#define LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_3
#define LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_4
#define LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_5
#endif


#define LIBYUC_CONTAINER_HASH_TABLE_DECLARATION(hash_table_type_name, offset_type, element_type, key_type) \
    typedef struct _##hash_table_type_name##HashTableIterator{ \
        offset_type cur_index; \
        offset_type entry_prev_id; \
        offset_type entry_cur_id; \
        LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DECLARATION \
    } hash_table_type_name##HashTableIterator; \
    \
    element_type* hash_table_type_name##HashTableIteratorFirst(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter); \
    element_type* hash_table_type_name##HashTableIteratorNext(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter); \
    element_type* hash_table_type_name##HashTableIteratorLocate(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter, const key_type* key); \
    void hash_table_type_name##HashTableIteratorPut(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter, const element_type* obj); \
    bool hash_table_type_name##HashTableIteratorDelete(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter); \
    \
    \
    LIBYUC_CONTAINER_SINGLY_LIST_DECLARATION(hash_table_type_name##HashLink, offset_type, offset_type) \
    typedef struct _##hash_table_type_name##HashLinkEntry { \
        union { \
            offset_type next; \
            hash_table_type_name##HashLinkSinglyListEntry entry; \
        }; \
        element_type obj; \
    } hash_table_type_name##HashLinkEntry; \
    typedef struct _##hash_table_type_name##HashLinkMainObj { /* 传递时的主对象 */ \
        hash_table_type_name##HashLinkSinglyListHead head; \
        struct _##hash_table_type_name##HashTable* table; \
    } hash_table_type_name##HashLinkMainObj; \
    \
    LIBYUC_CONTAINER_STATIC_LIST_DECLARATION(hash_table_type_name##HashLink, offset_type, hash_table_type_name##HashLinkEntry, 1) \
    LIBYUC_CONTAINER_VECTOR_DECLARATION(hash_table_type_name##HashLink, offset_type, hash_table_type_name##HashLinkEntry) \
    \
    LIBYUC_CONTAINER_VECTOR_DECLARATION(hash_table_type_name##HashBucket, offset_type, struct _##hash_table_type_name##HashTableEntry) \
    \
    typedef struct _##hash_table_type_name##HashTableEntry { \
        union { \
            hash_table_type_name##HashLinkSinglyListHead list_head; \
        }; \
    } hash_table_type_name##HashTableEntry; \
    typedef struct _##hash_table_type_name##HashTable { \
        hash_table_type_name##HashBucketVector bucket; \
        hash_table_type_name##HashLinkVector link; \
        offset_type load_fator; \
        offset_type mask; \
    } ##hash_table_type_name##HashTable; \
    \
    void hash_table_type_name##HashTableInit(hash_table_type_name##HashTable* table, offset_type capacity, offset_type load_fator); \
    void hash_table_type_name##HashTableRelease(hash_table_type_name##HashTable* table); \
    offset_type hash_table_type_name##HashTableGetCount(hash_table_type_name##HashTable* table); \
    element_type* hash_table_type_name##HashTableFind(hash_table_type_name##HashTable* table, const key_type* key); \
    void hash_table_type_name##HashTablePut(hash_table_type_name##HashTable* table, const element_type* obj); \
    bool hash_table_type_name##HashTableDelete(hash_table_type_name##HashTable* table, const key_type* key); \



// 访问器需要提供_GetKey方法
#define LIBYUC_CONTAINER_HASH_TABLE_DEFINE(hash_table_type_name, offset_type, element_type, key_type, allocator, accessor, obj_mover, hasher, comparer) \
    /*
    * 静态链表
    */ \
    static hash_table_type_name##HashLinkStaticList* hash_table_type_name##HashLinkGetStaticList(hash_table_type_name##HashLinkVector* link_vector) { \
        return (hash_table_type_name##HashLinkStaticList*)((uintptr_t)&link_vector->obj_arr[1] - sizeof(hash_table_type_name##HashLinkStaticList)); \
    } \
    LIBYUC_CONTAINER_STATIC_LIST_DEFINE(hash_table_type_name##HashLink, offset_type, hash_table_type_name##HashLinkEntry, LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_ACCESSOR, LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER, 1) \
    \
    static const offset_type hash_table_type_name##HashLinkReferencer_InvalidId = (-1); \
    forceinline hash_table_type_name##HashLinkSinglyListEntry* hash_table_type_name##HashLinkReferencer_Reference(hash_table_type_name##HashLinkSinglyListHead* head, offset_type entry_id) { \
        if (entry_id == hash_table_type_name##HashLinkReferencer_InvalidId) return NULL; \
        hash_table_type_name##HashLinkMainObj* obj = (hash_table_type_name##HashLinkMainObj*)head; \
        return &obj->table->link.obj_arr[entry_id + 1].entry; \
    } \
    forceinline void hash_table_type_name##HashLinkReferencer_Dereference(hash_table_type_name##HashLinkSinglyListHead* head, hash_table_type_name##HashLinkSinglyListEntry* entry) {    } \
    LIBYUC_CONTAINER_SINGLY_LIST_DEFINE(hash_table_type_name##HashLink, offset_type, offset_type, hash_table_type_name##HashLinkReferencer) \
    /*
    * 动态数组
    */ \
    forceinline void hash_table_type_name##HashLinkVectorCallbacker##_Expand(hash_table_type_name##HashLinkVector* arr, size_t old_capacity, size_t new_capacity) { \
        hash_table_type_name##HashLinkStaticListExpand(hash_table_type_name##HashLinkGetStaticList(arr), old_capacity, new_capacity); \
    } \
    LIBYUC_CONTAINER_VECTOR_DEFINE(hash_table_type_name##HashLink, offset_type, hash_table_type_name##HashLinkEntry, allocator, hash_table_type_name##HashLinkVectorCallbacker) \
    LIBYUC_CONTAINER_VECTOR_DEFINE(hash_table_type_name##HashBucket, offset_type, hash_table_type_name##HashTableEntry, allocator, LIBYUC_CONTAINER_VECTOR_DEFAULT_CALLBACKER) \
    \
    /*
    * 哈希表
    */ \
    static forceinline offset_type hash_table_type_name##HashModIndex(hash_table_type_name##HashTable* table, offset_type index) { \
        return index & table->mask; \
    } \
    static forceinline offset_type hash_table_type_name##HashGetIndex(hash_table_type_name##HashTable* table, const key_type* key) { \
        return hash_table_type_name##HashModIndex(table, hasher(table, key)); \
    } \
    static forceinline offset_type hash_table_type_name##HashGetCurrentLoadFator(hash_table_type_name##HashTable* table) { \
        return table->bucket.count * 100 / table->bucket.capacity; \
    } \
    static offset_type hash_table_type_name##HashTableAllocLinkEntry(hash_table_type_name##HashTable* table) { \
        hash_table_type_name##HashLinkStaticList* static_list = hash_table_type_name##HashLinkGetStaticList(&table->link); \
        offset_type id = hash_table_type_name##HashLinkStaticListPop(static_list, 0); \
         assert(id != LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER_InvalidId); \
        return id; \
    } \
    static void hash_table_type_name##HashTableFreeLinkEntry(hash_table_type_name##HashTable* table, offset_type id) { \
        hash_table_type_name##HashLinkStaticList* static_list = hash_table_type_name##HashLinkGetStaticList(&table->link); \
        hash_table_type_name##HashLinkStaticListPush(static_list, 0, id); \
    } \
    offset_type hash_table_type_name##HashTableGetCount(hash_table_type_name##HashTable* table){ return table->bucket.count; } \
    /* 重映射 */ \
    static void hash_table_type_name##HashRehash(hash_table_type_name##HashTable* table, offset_type new_capacity) {    \
        hash_table_type_name##HashTable temp_table; \
        hash_table_type_name##HashTableInit(&temp_table, new_capacity, table->load_fator); \
        hash_table_type_name##HashTableIterator iter; \
        element_type* obj = hash_table_type_name##HashTableIteratorFirst(table, &iter); \
        while (obj) { \
            hash_table_type_name##HashTablePut(&temp_table, obj); \
            key_type* key = accessor##_GetKey(table, obj); \
            obj = hash_table_type_name##HashTableIteratorNext(table, &iter); \
        } \
        hash_table_type_name##HashTableRelease(table); \
        MemoryCopy(table, &temp_table, sizeof(temp_table)); \
    } \
    void hash_table_type_name##HashTableInit(hash_table_type_name##HashTable* table, offset_type capacity, offset_type load_fator) { \
        if (capacity < 2) { \
            capacity = LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE; \
        } \
        hash_table_type_name##HashBucketVectorInit(&table->bucket, capacity, NULL); \
        hash_table_type_name##HashLinkVectorInit(&table->link, capacity + 1, NULL); \
        hash_table_type_name##HashLinkStaticListInit(hash_table_type_name##HashLinkGetStaticList(&table->link), capacity); \
        table->bucket.count = 0; \
        \
        for (offset_type i = 0; i < table->bucket.capacity; i++) { \
            hash_table_type_name##HashLinkSinglyListHeadInit(&table->bucket.obj_arr[i].list_head); \
        } \
        if (load_fator == 0) { \
            load_fator = LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR; \
        } \
        table->mask = 0; \
        while(capacity /= 2) {; \
            table->mask = (table->mask << 1) | 1; \
        } \
        table->load_fator = load_fator; \
    } \
    void hash_table_type_name##HashTableRelease(hash_table_type_name##HashTable* table) { \
        hash_table_type_name##HashBucketVectorRelease(&table->bucket); \
        hash_table_type_name##HashLinkVectorRelease(&table->link); \
    } \
    element_type* hash_table_type_name##HashTableFind(hash_table_type_name##HashTable* table, const key_type* key) { \
        hash_table_type_name##HashTableIterator iter; \
        return hash_table_type_name##HashTableIteratorLocate(table, &iter, key); \
    } \
    void hash_table_type_name##HashTablePut(hash_table_type_name##HashTable* table, const element_type* obj) { \
        key_type* key = accessor##_GetKey(table, obj); \
        hash_table_type_name##HashTableIterator iter; \
        hash_table_type_name##HashTableIteratorLocate(table, &iter, key); \
        hash_table_type_name##HashTableIteratorPut(table, &iter, obj); \
    } \
    bool hash_table_type_name##HashTableDelete(hash_table_type_name##HashTable* table, const key_type* key) { \
        hash_table_type_name##HashTableIterator iter; \
        hash_table_type_name##HashTableIteratorLocate(table, &iter, key); \
        return hash_table_type_name##HashTableIteratorDelete(table, &iter); \
    } \
    \
    forceinline element_type* hash_table_type_name##HashTableIteratorLocate(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter, const key_type* key) { \
        offset_type index = hash_table_type_name##HashGetIndex(table, key); \
        iter->cur_index = index; \
        hash_table_type_name##HashTableEntry* entry = &table->bucket.obj_arr[index]; \
        hash_table_type_name##HashLinkMainObj main_obj; \
        main_obj.head = entry->list_head; \
        main_obj.table = table; \
        iter->entry_cur_id = hash_table_type_name##HashLinkSinglyListIteratorFirst(&main_obj.head); \
        iter->entry_prev_id = hash_table_type_name##HashLinkReferencer_InvalidId; \
        while (iter->entry_cur_id != hash_table_type_name##HashLinkReferencer_InvalidId) { \
            hash_table_type_name##HashLinkEntry* link_entry = &table->link.obj_arr[iter->entry_cur_id + 1]; \
            if (comparer##_Equal(table, accessor##_GetKey(table, &link_entry->obj), key)) { \
                return &link_entry->obj; \
            } \
            iter->entry_prev_id = iter->entry_cur_id; \
            iter->entry_cur_id = hash_table_type_name##HashLinkSinglyListIteratorNext(&main_obj.head, iter->entry_cur_id); \
        } \
        return NULL; \
    } \
    void hash_table_type_name##HashTableIteratorPut(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter, const element_type* obj) { \
        hash_table_type_name##HashTableEntry* entry = &table->bucket.obj_arr[iter->cur_index]; \
        hash_table_type_name##HashLinkMainObj main_obj; \
        main_obj.table = table; \
        main_obj.head = entry->list_head; \
        if (iter->entry_cur_id != hash_table_type_name##HashLinkReferencer_InvalidId) { \
            hash_table_type_name##HashLinkEntry* link_entry = (hash_table_type_name##HashLinkEntry*)hash_table_type_name##HashLinkReferencer_Reference(&main_obj.head, iter->entry_cur_id); \
            if (comparer##_Equal(table, accessor##_GetKey(table, &link_entry->obj), accessor##_GetKey(table, obj))) { \
                obj_mover##_Copy(table, &link_entry->obj, obj); \
                return; \
            } \
            hash_table_type_name##HashLinkReferencer_Dereference(&main_obj.head, link_entry); \
        } \
        offset_type link_entry_id = hash_table_type_name##HashTableAllocLinkEntry(table); \
        obj_mover##_Copy(table, &table->link.obj_arr[link_entry_id + 1].obj, obj); \
        hash_table_type_name##HashLinkSinglyListPutFirst(&main_obj.head, link_entry_id); \
        table->bucket.count++; \
        entry->list_head = main_obj.head; \
        \
        if (hash_table_type_name##HashGetCurrentLoadFator(table) >= table->load_fator) { \
            /* 触发扩容 */ \
            hash_table_type_name##HashRehash(table, table->bucket.capacity * LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR); \
        } \
    } \
    bool hash_table_type_name##HashTableIteratorDelete(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
        hash_table_type_name##HashTableEntry* entry = &table->bucket.obj_arr[iter->cur_index]; \
        hash_table_type_name##HashLinkMainObj main_obj; \
        main_obj.head = entry->list_head; \
        main_obj.table = table; \
        offset_type link_entry_id = iter->entry_cur_id; \
        if (link_entry_id == hash_table_type_name##HashLinkReferencer_InvalidId) return false; \
        hash_table_type_name##HashLinkSinglyListDeleteEntry(&main_obj.head, iter->entry_prev_id, link_entry_id); \
        element_type* old_element = &table->link.obj_arr[link_entry_id + 1].obj; \
        table->bucket.count--; \
        entry->list_head = main_obj.head; \
        hash_table_type_name##HashTableFreeLinkEntry(table, link_entry_id); \
        return true; \
    } \
    element_type* hash_table_type_name##HashTableIteratorFirst(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
        iter->cur_index = 0; \
        iter->entry_cur_id = hash_table_type_name##HashLinkReferencer_InvalidId; \
        LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_1 \
        return hash_table_type_name##HashTableIteratorNext(table, iter); \
    } \
    element_type* hash_table_type_name##HashTableIteratorNext(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
        hash_table_type_name##HashLinkMainObj main_obj; \
        main_obj.table = table; \
        do { \
            if ((int32_t)iter->cur_index >= (int32_t)table->bucket.capacity) { \
                return NULL; \
            } \
            if (iter->entry_cur_id == hash_table_type_name##HashLinkReferencer_InvalidId) { \
                main_obj.head = table->bucket.obj_arr[iter->cur_index].list_head; \
                iter->entry_cur_id = hash_table_type_name##HashLinkSinglyListIteratorFirst(&main_obj.head); \
                iter->entry_prev_id = hash_table_type_name##HashLinkReferencer_InvalidId; \
            } else { \
                main_obj.head = table->bucket.obj_arr[iter->cur_index].list_head; \
                iter->entry_prev_id = iter->entry_cur_id; \
                iter->entry_cur_id = hash_table_type_name##HashLinkSinglyListIteratorNext(&main_obj.head, iter->entry_cur_id); \
            } \
            if (iter->entry_cur_id == hash_table_type_name##HashLinkReferencer_InvalidId) { \
                ++iter->cur_index; \
                continue; \
            } \
            break; \
        } while (true); \
        element_type* element = &table->link.obj_arr[iter->entry_cur_id + 1].obj; \
        return element; \
    } \


#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR_GetKey(MAIN_OBJ, OBJ) (OBJ)
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_HASHER(TABLE, KEY) HashCode_hashint(KEY)


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_HASH_TABLE_H_