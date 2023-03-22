/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_HASH_TABLE_H_
#define CUTILS_CONTAINER_HASH_TABLE_H_

#include <CUtils/object.h>
#include <CUtils/algorithm/hash_map.h>
#include <CUtils/container/vector.h>
#include <CUtils/container/singly_list.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 链式哈希表
*/
#define CUTILS_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE 16
#define CUTILS_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR 75//%
#define CUTILS_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR 2

#ifdef CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS
#define CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DECLARATION \
        uint32_t obj_count; \
        uint32_t free_count; \
        uint32_t list_entry_count; \
        uint32_t list_head_count; \
        uint32_t max_list_count; \
        uint32_t cur_list_count; \

#define CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_1 \
        iter->obj_count = 0; \
        iter->free_count = 0; \
        iter->list_entry_count = 0; \
        iter->list_head_count = 0; \
        iter->max_list_count = 0; \
        iter->cur_list_count = 0; \

#define CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_2 \
        iter->cur_list_count++; \
        if (iter->cur_list_count > iter->max_list_count) { \
            \
                iter->max_list_count = iter->cur_list_count; \
        } \
        iter->list_entry_count++; \

#define CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_3 \
        iter->free_count++;

#define CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_4 \
        iter->obj_count++; \

#define CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_5 \
        iter->cur_list_count = 0; \
        iter->list_head_count++; \

#else
#define CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DECLARATION 
#define CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_1
#define CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_2
#define CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_3
#define CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_4
#define CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_5
#endif


typedef enum _HashEntryType {
    kHashEntryFree,
    kHashEntryObj,
    kHashEntryList,
} HashEntryType;


#define CUTILS_CONTAINER_HASH_TABLE_DECLARATION(hash_table_type_name, element_type, key_type) \
    CUTILS_CONTAINER_VECTOR_DECLARATION(hash_table_type_name##HashTable, struct _##hash_table_type_name##HashEntry) \
    \
    typedef struct _##hash_table_type_name##HashTableIterator{ \
        uint32_t cur_index; \
        struct _##hash_table_type_name##HashEntryList* cur_list_entry; \
        CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DECLARATION \
    } hash_table_type_name##HashTableIterator; \
    \
    element_type* hash_table_type_name##HashTableIteratorFirst(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter); \
    element_type* hash_table_type_name##HashTableIteratorNext(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter); \
    \
    typedef struct _##hash_table_type_name##HashEntry { \
        HashEntryType type; \
        union { \
            element_type obj; \
            SinglyListHead list_head; \
        }; \
    } hash_table_type_name##HashEntry; \
    typedef struct _##hash_table_type_name##HashEntryList { \
        SinglyListEntry list_entry; \
        element_type obj; \
    } hash_table_type_name##HashEntryList; \
    typedef struct _##hash_table_type_name##HashTable { \
        hash_table_type_name##HashTableVector bucket; \
        /* hash_table_type_name##HashTableVector temp_bucket;        // 保留，可优化为逐渐搬迁 */ \
        uint32_t load_fator; \
    } ##hash_table_type_name##HashTable; \
    \
    void hash_table_type_name##HashTableInit(hash_table_type_name##HashTable* table, size_t capacity, uint32_t load_fator); \
    element_type* hash_table_type_name##HashTableFind(hash_table_type_name##HashTable* table, const key_type* key); \
    bool hash_table_type_name##HashTablePut(hash_table_type_name##HashTable* table, const element_type* obj); \
    bool hash_table_type_name##HashTableDelete(hash_table_type_name##HashTable* table, const key_type* key); \



// 访问器需要提供_GetKey方法
#define CUTILS_CONTAINER_HASH_TABLE_DEFINE(hash_table_type_name, element_type, key_type, allocator, accessor, mover_obj, hasher, comparer) \
    CUTILS_CONTAINER_VECTOR_DEFINE(hash_table_type_name##HashTable, hash_table_type_name##HashEntry, allocator) \
    static inline uint32_t hash_table_type_name##HashGetIndex(hash_table_type_name##HashTable* table, const key_type* key) { \
        return hasher(table, *key) % table->bucket.capacity; \
    } \
    static inline uint32_t hash_table_type_name##HashGetCurrentLoadFator(hash_table_type_name##HashTable* table) { \
        return table->bucket.count * 100 / table->bucket.capacity; \
    } \
    static void hash_table_type_name##HashRehash(hash_table_type_name##HashTable* table, size_t new_capacity) {  \
        hash_table_type_name##HashTable temp_table; \
        hash_table_type_name##HashTableInit(&temp_table, new_capacity, table->load_fator); \
        /* 重映射 */ \
        /* 优化点之一，可以添加基于迭代器删除节点的接口 */ \
        hash_table_type_name##HashTableIterator iter; \
        element_type* obj = hash_table_type_name##HashTableIteratorFirst(table, &iter); \
        while (obj) { \
            hash_table_type_name##HashTablePut(&temp_table, obj); \
            hash_table_type_name##HashTableDelete(table, &accessor##_GetKey(table, *obj)); \
            obj = hash_table_type_name##HashTableIteratorNext(table, &iter); \
        } \
        \
        hash_table_type_name##HashTableVectorRelease(&table->bucket); \
        MemoryCopy(table, &temp_table, sizeof(temp_table)); \
    } \
    void hash_table_type_name##HashTableInit(hash_table_type_name##HashTable* table, size_t capacity, uint32_t load_fator) { \
        if (capacity == 0) { \
            capacity = CUTILS_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE; \
        } \
        hash_table_type_name##HashTableVectorInit(&table->bucket, capacity, true); \
        table->bucket.count = 0; \
        \
        for (int i = 0; i < table->bucket.capacity; i++) { \
            table->bucket.obj_arr[i].type = kHashEntryFree; \
        } \
        if (load_fator == 0) { \
            load_fator = CUTILS_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR; \
        } \
        table->load_fator = load_fator; \
    } \
    void hash_table_type_name##HashTableRelease(hash_table_type_name##HashTable* table) { \
        hash_table_type_name##HashTableIterator iter; \
        element_type* obj = hash_table_type_name##HashTableIteratorFirst(table, &iter); \
        while (obj) { \
            hash_table_type_name##HashTableDelete(table, &accessor##_GetKey(table, *obj)); \
            obj = hash_table_type_name##HashTableIteratorNext(table, &iter); \
        } \
        hash_table_type_name##HashTableVectorRelease(&table->bucket); \
    } \
    element_type* hash_table_type_name##HashTableFind(hash_table_type_name##HashTable* table, const key_type* key) { \
        uint32_t index = hash_table_type_name##HashGetIndex(table, key); \
        hash_table_type_name##HashEntry* entry = &table->bucket.obj_arr[index]; \
        if (entry->type == kHashEntryObj) { \
            if(comparer##_Equal(table, accessor##_GetKey(table, entry->obj), *key)) { \
                return &entry->obj; \
            } \
        } \
        else if (entry->type == kHashEntryList) { \
            SinglyListEntry* cur = SinglyListIteratorFirst(&entry->list_head); \
            while (cur) { \
                hash_table_type_name##HashEntryList* list_entry = (hash_table_type_name##HashEntryList*)cur; \
                if (comparer##_Equal(table, accessor##_GetKey(table, list_entry->obj), *key)) { \
                    return &entry->obj; \
                } \
                cur = SinglyListIteratorNext(&entry->list_head, cur); \
            } \
        } \
        return NULL; \
    } \
    bool hash_table_type_name##HashTablePut(hash_table_type_name##HashTable* table, const element_type* obj) { \
        uint32_t index = hash_table_type_name##HashGetIndex(table, obj); \
        hash_table_type_name##HashEntry* entry = &table->bucket.obj_arr[index]; \
        \
        if (entry->type == kHashEntryFree) { \
            mover_obj##_Assignment(table, entry->obj, *obj); \
            entry->type = kHashEntryObj; \
        } \
        else if (entry->type == kHashEntryObj) { \
            if (comparer##_Equal(table, accessor##_GetKey(table, entry->obj), accessor##_GetKey(table, *obj))) { \
                mover_obj##_Assignment(table, entry->obj, *obj); \
                return true; \
            } \
            entry->type = kHashEntryList; \
            hash_table_type_name##HashEntryList* list_entry = allocator##_Create(table, hash_table_type_name##HashEntryList); \
            mover_obj##_Assignment(table, list_entry->obj, entry->obj); \
            SinglyListHeadInit(&entry->list_head); \
            SinglyListPutFirst(&entry->list_head, &list_entry->list_entry); \
            list_entry = allocator##_Create(table, hash_table_type_name##HashEntryList); \
            mover_obj##_Assignment(table, list_entry->obj, *obj); \
            SinglyListPutFirst(&entry->list_head, &list_entry->list_entry); \
        } \
        else if (entry->type == kHashEntryList) { \
            SinglyListEntry* cur = SinglyListIteratorFirst(&entry->list_head); \
            while (cur) { \
                hash_table_type_name##HashEntryList* list_entry = (hash_table_type_name##HashEntryList*)cur; \
                if (comparer##_Equal(table, accessor##_GetKey(table, list_entry->obj), accessor##_GetKey(table, *obj))) { \
                    mover_obj##_Assignment(table, list_entry->obj, *obj); \
                    break; \
                } \
                cur = SinglyListIteratorNext(&entry->list_head, cur); \
            } \
            if (!cur) { \
                hash_table_type_name##HashEntryList* list_entry = allocator##_Create(table, hash_table_type_name##HashEntryList); \
                mover_obj##_Assignment(table, list_entry->obj, *obj); \
                SinglyListPutFirst(&entry->list_head, &list_entry->list_entry); \
            } \
        } \
        \
        table->bucket.count++; \
        if (hash_table_type_name##HashGetCurrentLoadFator(table) >= table->load_fator) { \
            /* 触发扩容 */ \
            hash_table_type_name##HashRehash(table, table->bucket.capacity * CUTILS_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR); \
        } \
        return true; \
    } \
    bool hash_table_type_name##HashTableDelete(hash_table_type_name##HashTable* table, const key_type* key) { \
        uint32_t index = hash_table_type_name##HashGetIndex(table, key); \
        hash_table_type_name##HashEntry* entry = &table->bucket.obj_arr[index]; \
        bool success = true; \
        if (entry->type == kHashEntryFree) { \
            return false; \
        } \
        else if (entry->type == kHashEntryObj) { \
            if (!comparer##_Equal(table, accessor##_GetKey(table, entry->obj), *key)) { \
                return false; \
            } \
            entry->type = kHashEntryFree; \
            success = true; \
        } \
        else if (entry->type == kHashEntryList) { \
            SinglyListEntry* prev = NULL; \
            SinglyListEntry* cur = SinglyListIteratorFirst(&entry->list_head); \
            while (cur) { \
                if (!comparer##_Equal(table, accessor##_GetKey(table, ((hash_table_type_name##HashEntryList*)cur)->obj), *key)) { \
                    prev = cur; \
                    cur = SinglyListIteratorNext(&entry->list_head, cur); \
                    continue; \
                } \
                if (prev) { \
                    SinglyListDeleteEntry(&entry->list_head, prev, cur); \
                } \
                else { \
                    SinglyListDeleteFirst(&entry->list_head); \
                    if (SinglyListIsEmpty(&entry->list_head)) { \
                        entry->type = kHashEntryFree; \
                    } \
                } \
                allocator##_Delete(table, cur); \
                success = true; \
                break; \
            } \
        } \
        if (success) table->bucket.count--; \
        return success; \
    } \
    /* 
    * 现在的迭代思路是遍历空间所有节点，另外可以用静态链表连接所有已映射的节点，但需要额外空间
    */ \
    element_type* hash_table_type_name##HashTableIteratorFirst(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
        iter->cur_list_entry = NULL; \
        iter->cur_index = 0; \
        CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_1 \
        return hash_table_type_name##HashTableIteratorNext(table, iter); \
    } \
    element_type* hash_table_type_name##HashTableIteratorNext(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
        if (iter->cur_list_entry) { \
            hash_table_type_name##HashEntryList* cur = iter->cur_list_entry; \
            hash_table_type_name##HashEntry* entry = &table->bucket.obj_arr[iter->cur_index]; \
            iter->cur_list_entry = (hash_table_type_name##HashEntryList*)SinglyListIteratorNext(&entry->list_head, &cur->list_entry); \
            if (iter->cur_list_entry == NULL) { \
                iter->cur_index++; \
            } \
            if (cur) { \
                CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_2 \
                return &cur->obj; \
            } \
        } \
        for (; iter->cur_index < table->bucket.capacity; iter->cur_index++) { \
            hash_table_type_name##HashEntry* entry = &table->bucket.obj_arr[iter->cur_index]; \
            if (entry->type == kHashEntryFree) { \
                CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_3 \
                continue; \
            } \
            if (entry->type == kHashEntryObj) { \
                CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_4 \
                iter->cur_index++; \
                return &entry->obj; \
            } \
            if (entry->type == kHashEntryList) { \
                CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_5 \
                iter->cur_list_entry = (hash_table_type_name##HashEntryList*)entry->list_head.first; \
                return hash_table_type_name##HashTableIteratorNext(table, iter); \
            } \
        } \
        return NULL; \
    } \
 


//CUTILS_CONTAINER_HASH_TABLE_DECLARATION(Int, int, int)
//#define INT_HASHTABLE_ACCESSOR_GetKey(obj) (obj)
//#define INT_HASHTABLE_ACCESSOR INT_HASHTABLE_ACCESSOR
//#define INT_HASHTABLE_HASHER_KEY(key) Hashmap_hashint(key)
//CUTILS_CONTAINER_HASH_TABLE_DEFINE(Int, int, int, CUTILS_OBJECT_ALLOCATOR_DEFALUT, INT_HASHTABLE_ACCESSOR, CUTILS_OBJECT_MOVER_DEFALUT, INT_HASHTABLE_HASHER_KEY, CUTILS_OBJECT_COMPARER_DEFALUT)

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_HASH_TABLE_H_