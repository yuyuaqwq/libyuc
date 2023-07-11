/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_HASH_TABLE_H_
#define LIBYUC_CONTAINER_HASH_TABLE_H_

#include <libyuc/object.h>
#include <libyuc/algorithm/hash_code.h>
#include <libyuc/container/vector.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 基于开放定址(线性探测)法的哈希表
*/
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE 16
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR 75  //%
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR 2
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_MAX_DETECTION_COUNT 8  // (table->bucket.capacity - 1)


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
            \
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


#define LIBYUC_CONTAINER_HASH_TABLE_DECLARATION(hash_table_type_name, element_type, key_type) \
    typedef struct _##hash_table_type_name##HashTableIterator{ \
        uint32_t cur_index; \
        LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DECLARATION \
    } hash_table_type_name##HashTableIterator; \
    \
    element_type* hash_table_type_name##HashTableIteratorFirst(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter); \
    element_type* hash_table_type_name##HashTableIteratorNext(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter); \
    element_type* hash_table_type_name##HashTableIteratorLocate(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter, const key_type* key); \
    void hash_table_type_name##HashTableIteratorPut(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter, const element_type* obj); \
    bool hash_table_type_name##HashTableIteratorDelete(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter); \
    \
    LIBYUC_CONTAINER_VECTOR_DECLARATION(hash_table_type_name##HashBucket, struct _##hash_table_type_name##HashTableEntry) \
    \
    typedef struct _##hash_table_type_name##HashTableEntry { \
        element_type obj; \
    } hash_table_type_name##HashTableEntry; \
    typedef struct _##hash_table_type_name##HashTable { \
        hash_table_type_name##HashBucketVector bucket; \
        uint32_t load_fator; \
        key_type empty_key; \
        element_type empty_obj; \
    } ##hash_table_type_name##HashTable; \
    \
    void hash_table_type_name##HashTableInit(hash_table_type_name##HashTable* table, size_t capacity, uint32_t load_fator, const element_type* empty_obj, const key_type* empty_key); \
    void hash_table_type_name##HashTableRelease(hash_table_type_name##HashTable* table); \
    size_t hash_table_type_name##HashTableGetCount(hash_table_type_name##HashTable* table); \
    element_type* hash_table_type_name##HashTableFind(hash_table_type_name##HashTable* table, const key_type* key); \
    void hash_table_type_name##HashTablePut(hash_table_type_name##HashTable* table, const element_type* obj); \
    bool hash_table_type_name##HashTableDelete(hash_table_type_name##HashTable* table, const key_type* key); \



// 访问器需要提供_GetKey方法
#define LIBYUC_CONTAINER_HASH_TABLE_DEFINE(hash_table_type_name, element_type, key_type, allocator, accessor, element_mover, key_mover, hasher, comparer) \
    /*
    * 动态数组
    */ \
    LIBYUC_CONTAINER_VECTOR_DEFINE(hash_table_type_name##HashBucket, hash_table_type_name##HashTableEntry, allocator, LIBYUC_CONTAINER_VECTOR_DEFAULT_CALLBACKER) \
    \
    /*
    * 哈希表
    */ \
    static forceinline uint32_t hash_table_type_name##HashGetIndex(hash_table_type_name##HashTable* table, const key_type* key) { \
        return hasher(table, key) % table->bucket.capacity; \
    } \
    static forceinline uint32_t hash_table_type_name##HashGetCurrentLoadFator(hash_table_type_name##HashTable* table) { \
        return table->bucket.count * 100 / table->bucket.capacity; \
    } \
    size_t hash_table_type_name##HashTableGetCount(hash_table_type_name##HashTable* table){ return table->bucket.count; } \
    /* 重映射 */ \
    static void hash_table_type_name##HashRehash(hash_table_type_name##HashTable* table, size_t new_capacity) {  \
        hash_table_type_name##HashTable temp_table; \
        hash_table_type_name##HashTableInit(&temp_table, new_capacity, table->load_fator, &table->empty_obj, &table->empty_key); \
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
    void hash_table_type_name##HashTableInit(hash_table_type_name##HashTable* table, size_t capacity, uint32_t load_fator, const element_type* empty_obj, const key_type* empty_key) { \
        if (capacity == 0) { \
            capacity = LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE; \
        } \
        hash_table_type_name##HashBucketVectorInit(&table->bucket, capacity, true); \
        table->bucket.count = 0; \
        table->empty_key = *empty_key; \
        table->empty_obj = *empty_obj; \
        \
        for (int i = 0; i < table->bucket.capacity; i++) { \
            memcpy(&table->bucket.obj_arr[i].obj, empty_obj, sizeof(*empty_obj)); \
        } \
        if (load_fator == 0) { \
            load_fator = LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR; \
        } \
        table->load_fator = load_fator; \
    } \
    void hash_table_type_name##HashTableRelease(hash_table_type_name##HashTable* table) { \
        hash_table_type_name##HashBucketVectorRelease(&table->bucket); \
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
        iter->cur_index = hash_table_type_name##HashGetIndex(table, key); \
        for (int32_t i = 0; i < LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_MAX_DETECTION_COUNT; i++) { \
            hash_table_type_name##HashTableEntry* entry = &table->bucket.obj_arr[iter->cur_index]; \
            key_type* entry_key = accessor##_GetKey(table, &entry->obj); \
            if (comparer##_Equal(table, entry_key, key)) { \
                return &entry->obj; \
            } \
            if (comparer##_Equal(table, entry_key, &table->empty_key)) { \
                break; \
            } \
            if (i + 1 != LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_MAX_DETECTION_COUNT) \
                iter->cur_index = (++iter->cur_index) % table->bucket.capacity; \
        } \
        return NULL; \
    } \
    void hash_table_type_name##HashTableIteratorPut(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter, const element_type* obj) { \
        hash_table_type_name##HashTableEntry* entry = &table->bucket.obj_arr[iter->cur_index]; \
        if (comparer##_Equal(table, accessor##_GetKey(table, &entry->obj), accessor##_GetKey(table, obj))) { \
            element_mover##_Copy(table, &entry->obj, obj); \
            return; \
        } \
        if (comparer##_Equal(table, accessor##_GetKey(table, &entry->obj), &table->empty_key)) { \
            element_mover##_Copy(table, &entry->obj, obj); \
        } \
        else { \
            do { \
                hash_table_type_name##HashRehash(table, table->bucket.capacity * LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR); \
                iter->cur_index = hash_table_type_name##HashGetIndex(table, accessor##_GetKey(table, obj)); \
                entry = &table->bucket.obj_arr[iter->cur_index]; \
                int32_t i = 0; \
                for (; i < LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_MAX_DETECTION_COUNT; i++) { \
                    key_type* entry_key = accessor##_GetKey(table, &entry->obj); \
                    if (comparer##_Equal(table, entry_key, &table->empty_key)) { \
                        element_mover##_Copy(table, &entry->obj, obj); \
                        break; \
                    } \
                    iter->cur_index = (++iter->cur_index) % table->bucket.capacity; \
                    entry = &table->bucket.obj_arr[iter->cur_index]; \
                } \
                if (i < LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_MAX_DETECTION_COUNT) { \
                    break; \
                } \
            } while (true); \
        } \
        table->bucket.count++; \
        if (hash_table_type_name##HashGetCurrentLoadFator(table) >= table->load_fator) { \
            /* 触发扩容 */ \
            hash_table_type_name##HashRehash(table, table->bucket.capacity * LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR); \
        } \
    } \
    bool hash_table_type_name##HashTableIteratorDelete(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
        hash_table_type_name##HashTableEntry* entry = &table->bucket.obj_arr[iter->cur_index]; \
        if (comparer##_Equal(table, accessor##_GetKey(table, &entry->obj), &table->empty_key)) { \
            return false; \
        } \
        key_mover##_Copy(table, accessor##_GetKey(table, &entry->obj), &table->empty_key); \
        table->bucket.count--; \
        return true; \
    } \
    element_type* hash_table_type_name##HashTableIteratorFirst(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
        iter->cur_index = 0; \
        LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_1 \
        return hash_table_type_name##HashTableIteratorNext(table, iter); \
    } \
    element_type* hash_table_type_name##HashTableIteratorNext(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
        hash_table_type_name##HashTableEntry* entry = NULL; \
        do { \
            if ((int32_t)iter->cur_index >= (int32_t)table->bucket.capacity) { \
                return NULL; \
            } \
            entry = &table->bucket.obj_arr[iter->cur_index++]; \
            if (!comparer##_Equal(table, accessor##_GetKey(table, &entry->obj), &table->empty_key)) { \
                break; \
            } \
        } while (true); \
        return &entry->obj; \
    } \


#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR_GetKey(MAIN_OBJ, OBJ) (OBJ)
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_HASHER(TABLE, KEY) HashCode_hashint(KEY)


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_HASH_TABLE_H_