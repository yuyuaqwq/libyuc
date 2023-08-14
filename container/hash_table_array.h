/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_HASH_TABLE_H_
#define LIBYUC_CONTAINER_HASH_TABLE_H_

#include <libyuc/basic.h>
#include <libyuc/algorithm/two.h>
#include <libyuc/algorithm/hash_code.h>
#include <libyuc/container/vector.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 链出动态数组的哈希表实现
*/
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE 16
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR 75    //%
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR 2
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_MAX_DETECTION_COUNT 12    // (table->bucket.capacity - 1)



#define LIBYUC_CONTAINER_HASH_TABLE_DECLARATION(hash_table_type_name, offset_type, element_type, key_type) \
    typedef struct _##hash_table_type_name##HashTableIterator{ \
        offset_type cur_index; \
    } hash_table_type_name##HashTableIterator; \
    \
    element_type* hash_table_type_name##HashTableIteratorFirst(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter); \
    element_type* hash_table_type_name##HashTableIteratorNext(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter); \
    element_type* hash_table_type_name##HashTableIteratorLocate(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter, const key_type* key); \
    bool hash_table_type_name##HashTableIteratorDelete(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter); \
    \
    LIBYUC_CONTAINER_VECTOR_DECLARATION( \
        hash_table_type_name##HashTableEntry, \
        LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC, \
        offset_type, \
        offset_type, \
        element_type) \
    LIBYUC_CONTAINER_VECTOR_DECLARATION( \
        hash_table_type_name##HashBucket, \
        LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC, \
        offset_type, \
        offset_type,\
        struct _##hash_table_type_name##HashTableEntry) \
    \
    typedef struct _##hash_table_type_name##HashTableEntry { \
        hash_table_type_name##HashTableEntryVector vector; \
    } hash_table_type_name##HashTableEntry; \
    typedef struct _##hash_table_type_name##HashTable { \
        offset_type max_detection_count; \
        offset_type load_fator; \
        offset_type shift; \
        hash_table_type_name##HashBucketVector bucket; \
    } ##hash_table_type_name##HashTable; \
    \
    bool hash_table_type_name##HashTableInit(hash_table_type_name##HashTable* table, offset_type capacity, offset_type load_fator); \
    void hash_table_type_name##HashTableRelease(hash_table_type_name##HashTable* table); \
    offset_type hash_table_type_name##HashTableGetCount(hash_table_type_name##HashTable* table); \
    element_type* hash_table_type_name##HashTableFind(hash_table_type_name##HashTable* table, const key_type* key); \
    void hash_table_type_name##HashTablePut(hash_table_type_name##HashTable* table, const element_type* obj); \
    bool hash_table_type_name##HashTableDelete(hash_table_type_name##HashTable* table, const key_type* key); \



// 访问器需要提供_GetKey、_SetKey方法
#define LIBYUC_CONTAINER_HASH_TABLE_DEFINE(hash_table_type_name, offset_type, element_type, key_type, allocator, accessor, element_mover, hasher, comparer) \
    /*
    * 动态数组
    */ \
    static const k##hash_table_type_name##HashTableInvalidDist = 0; \
    LIBYUC_CONTAINER_VECTOR_DEFINE(hash_table_type_name##HashBucket, \
        LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC, \
        offset_type, \
        offset_type, \
        hash_table_type_name##HashTableEntry, \
        allocator, \
        LIBYUC_CONTAINER_VECTOR_CALLBACKER_DEFAULT) \
    \
    /*
    * 哈希表
    */ \
    static forceinline offset_type hash_table_type_name##HashModIndex(hash_table_type_name##HashTable* table, offset_type index) { \
        return index & table->max_detection_count/* % table->bucket.capacity */; \
    } \
    static forceinline offset_type hash_table_type_name##HashGetIndex(hash_table_type_name##HashTable* table, const key_type* key) { \
        return hasher(table, key) >> table->shift; \
    } \
    static forceinline offset_type hash_table_type_name##HashGetCurrentLoadFator(hash_table_type_name##HashTable* table) { \
        return table->bucket.count * 100 / table->bucket.capacity; \
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
    bool hash_table_type_name##HashTableInit(hash_table_type_name##HashTable* table, offset_type capacity, offset_type load_fator) { \
        if (capacity < 2) { \
            capacity = LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE; \
        } \
        if (!LIBYUC_ALGORITHM_TWO_IS_POWER_OF_2(capacity)) { \
            capacity = LIBYUC_ALGORITHM_TWO_ALIGN_TO_POWER_OF_2(capacity); \
        } \
        hash_table_type_name##HashBucketVectorInit(&table->bucket, capacity, NULL); \
        table->bucket.count = 0; \
        \
        for (int i = 0; i < table->bucket.capacity; i++) { \
            hash_table_type_name##HashTableEntryVectorInit(&hash_table_type_name##HashBucketVectorIndex(&table->bucket, i)->vector, 0, NULL); \
        } \
        if (load_fator == 0) { \
            load_fator = LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR; \
        } \
        table->load_fator = load_fator; \
        table->max_detection_count = capacity - 1; \
        table->shift = sizeof(hasher(table, ((key_type*)NULL))) * 8; \
        while (capacity /= 2) table->shift--; \
        return true; \
    } \
    void hash_table_type_name##HashTableRelease(hash_table_type_name##HashTable* table) { \
        hash_table_type_name##HashBucketVectorRelease(&table->bucket); \
    } \
    forceinline element_type* hash_table_type_name##HashTableIteratorLocate(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter, const key_type* key) { \
        iter->cur_index = hash_table_type_name##HashGetIndex(table, key); \
        \
        hash_table_type_name##HashTableEntry* entry = hash_table_type_name##HashBucketVectorIndex(&table->bucket, iter->cur_index); \
        hash_table_type_name##HashTableEntryVectorIterator entry_iter; \
        element_type* ele = hash_table_type_name##HashTableEntryVectorIteratorFirst(&entry->vector, &entry_iter); \
        while (ele) { \
            key_type* entry_key = accessor##_GetKey(table, ele); \
            if (comparer##_Equal(table, entry_key, key)) { \
                return &entry->obj; \
            } \
            ele = hash_table_type_name##HashTableEntryVectorIteratorNext(&entry->vector, &entry_iter); \
        } \
        return NULL; \
    } \
    bool hash_table_type_name##HashTableIteratorDelete(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
        hash_table_type_name##HashTableEntry* empty_entry = hash_table_type_name##HashBucketVectorIndex(&table->bucket, iter->cur_index); \
        if (empty_entry == NULL) { \
            return false; \
        } \
        iter->cur_index = hash_table_type_name##HashModIndex(table, iter->cur_index + 1); \
        hash_table_type_name##HashTableEntry* next_entry = hash_table_type_name##HashBucketVectorIndex(&table->bucket, iter->cur_index); \
        while (next_entry->dist > 1) { \
            --next_entry->dist; \
            hash_table_type_name##HashEntryMove(table, empty_entry, next_entry); \
            empty_entry = next_entry; \
            iter->cur_index = hash_table_type_name##HashModIndex(table, iter->cur_index + 1); \
            next_entry = hash_table_type_name##HashBucketVectorIndex(&table->bucket, iter->cur_index); \
        } \
        empty_entry->dist = k##hash_table_type_name##HashTableInvalidDist; \
        table->bucket.count--; \
        if (table->bucket.capacity > LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE && hash_table_type_name##HashGetCurrentLoadFator(table) <= 100 - table->load_fator) { \
            hash_table_type_name##HashRehash(table, table->bucket.capacity / LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR); \
        } \
        return true; \
    } \
    element_type* hash_table_type_name##HashTableIteratorFirst(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
        iter->cur_index = 0; \
        return hash_table_type_name##HashTableIteratorNext(table, iter); \
    } \
    element_type* hash_table_type_name##HashTableIteratorNext(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
        hash_table_type_name##HashTableEntry* entry = NULL; \
        do { \
            if (iter->cur_index >= table->bucket.capacity) { \
                return NULL; \
            } \
            entry = &table->bucket.obj_arr[iter->cur_index++]; \
            if (entry->dist != k##hash_table_type_name##HashTableInvalidDist) { \
                break; \
            } \
        } while (true); \
        return &entry->obj; \
    } \
    \
    element_type* hash_table_type_name##HashTableFind(hash_table_type_name##HashTable* table, const key_type* key) { \
        hash_table_type_name##HashTableIterator iter; \
        return hash_table_type_name##HashTableIteratorLocate(table, &iter, key); \
    } \
    void hash_table_type_name##HashTablePut(hash_table_type_name##HashTable* table, const element_type* obj) { \
        offset_type dist = 1; \
        key_type* key = accessor##_GetKey(table, obj); \
        offset_type cur_index = hash_table_type_name##HashGetIndex(table, key); \
        hash_table_type_name##HashTableEntry* entry; \
        do { \
            entry = hash_table_type_name##HashBucketVectorIndex(&table->bucket, cur_index); \
            if (dist == entry->dist && comparer##_Equal(table, accessor##_GetKey(table, &entry->obj), key)) { \
                element_mover##_Copy(table, &entry->obj, obj); \
                return; \
            } \
            if (dist > entry->dist) break; \
            ++dist; \
            cur_index = hash_table_type_name##HashModIndex(table, cur_index + 1); \
        } while(true); \
        hash_table_type_name##HashTableEntry temp; \
        temp.dist = dist; \
        element_mover##_Copy(table, &temp.obj, obj); \
        while (entry->dist != 0) { \
            hash_table_type_name##HashEntryExchange(table, &temp, entry); \
            ++temp.dist; \
            cur_index = hash_table_type_name##HashModIndex(table, cur_index + 1); \
            entry = hash_table_type_name##HashBucketVectorIndex(&table->bucket, cur_index); \
        } \
        hash_table_type_name##HashEntryExchange(table, &temp, entry); \
        \
        table->bucket.count++; \
        if (hash_table_type_name##HashGetCurrentLoadFator(table) >= table->load_fator) { \
            /* 触发扩容 */ \
            hash_table_type_name##HashRehash(table, table->bucket.capacity * LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR); \
        } \
    } \
    bool hash_table_type_name##HashTableDelete(hash_table_type_name##HashTable* table, const key_type* key) { \
        hash_table_type_name##HashTableIterator iter; \
        hash_table_type_name##HashTableIteratorLocate(table, &iter, key); \
        return hash_table_type_name##HashTableIteratorDelete(table, &iter); \
    } \


#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR_GetKey(MAIN_OBJ, OBJ) (OBJ)
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR_SetKey(MAIN_OBJ, OBJ, KEY) (*(OBJ) = *(KEY))
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_HASHER(TABLE, KEY) HashCode_hashint(KEY)


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_HASH_TABLE_H_