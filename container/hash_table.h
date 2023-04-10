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
#include <CUtils/container/static_list.h>
#include <CUtils/container/rb_tree.h>

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
        kHashEntryTree,
    } HashEntryType;

#define CUTILS_CONTAINER_HASH_TABLE_DECLARATION(hash_table_type_name, element_type, key_type) \
    typedef struct _##hash_table_type_name##HashTableIterator{ \
        uint32_t cur_index; \
        struct _##hash_table_type_name##HashLinkEntry* cur_list_entry; \
        CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DECLARATION \
    } hash_table_type_name##HashTableIterator; \
    \
    element_type* hash_table_type_name##HashTableIteratorFirst(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter); \
    element_type* hash_table_type_name##HashTableIteratorNext(struct _##hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter); \
    \
    \
    CUTILS_CONTAINER_RB_TREE_DECLARATION(hash_table_type_name##HashLink, int32_t, key_type) \
    typedef struct _##hash_table_type_name##HashLinkEntry { \
        union  { \
            int32_t next; \
            hash_table_type_name##HashLinkRbEntry rb_entry; \
        }; \
        element_type obj; \
    } hash_table_type_name##HashLinkEntry; \
    typedef struct _##hash_table_type_name##HashLinkRbObj { /* 传递时的主对象 */ \
        hash_table_type_name##HashLinkRbTree rb_tree; \
        struct _##hash_table_type_name##HashTable* table; \
    } hash_table_type_name##HashLinkRbObj; \
    \
    CUTILS_CONTAINER_STATIC_LIST_DECLARATION(hash_table_type_name##HashLink, int32_t, hash_table_type_name##HashLinkEntry, 1) \
    CUTILS_CONTAINER_VECTOR_DECLARATION(hash_table_type_name##HashLink, hash_table_type_name##HashLinkEntry) \
    \
    CUTILS_CONTAINER_VECTOR_DECLARATION(hash_table_type_name##HashType, int8_t) \
    CUTILS_CONTAINER_VECTOR_DECLARATION(hash_table_type_name##HashBucket, struct _##hash_table_type_name##HashEntry) \
    \
    typedef struct _##hash_table_type_name##HashEntry { \
        union { \
            element_type obj; \
            hash_table_type_name##HashLinkRbTree rb_tree; \
        }; \
    } hash_table_type_name##HashEntry; \
    typedef struct _##hash_table_type_name##HashTable { \
        hash_table_type_name##HashTypeVector type; \
        hash_table_type_name##HashBucketVector bucket; \
        hash_table_type_name##HashLinkVector link; \
        /* hash_table_type_name##HashBucketVector temp_bucket;        // 保留，可优化为逐渐搬迁 */ \
        uint32_t load_fator; \
    } ##hash_table_type_name##HashTable; \
    \
    void hash_table_type_name##HashTableInit(hash_table_type_name##HashTable* table, size_t capacity, uint32_t load_fator); \
    size_t hash_table_type_name##HashTableGetCount(hash_table_type_name##HashTable* table); \
    element_type* hash_table_type_name##HashTableFind(hash_table_type_name##HashTable* table, const key_type* key); \
    bool hash_table_type_name##HashTablePut(hash_table_type_name##HashTable* table, const element_type* obj); \
    bool hash_table_type_name##HashTableDelete(hash_table_type_name##HashTable* table, const key_type* key); \



// 访问器需要提供_GetKey方法
#define CUTILS_CONTAINER_HASH_TABLE_DEFINE(hash_table_type_name, element_type, key_type, allocator, accessor, obj_mover, hasher, comparer) \
    static hash_table_type_name##HashLinkStaticList* hash_table_type_name##HashLinkGetStaticList(hash_table_type_name##HashLinkVector* link_vector) { \
        return (hash_table_type_name##HashLinkStaticList*)((uintptr_t)&link_vector->obj_arr[1] - sizeof(hash_table_type_name##HashLinkStaticList)); \
    } \
    CUTILS_CONTAINER_STATIC_LIST_DEFINE(hash_table_type_name##HashLink, int32_t, hash_table_type_name##HashLinkEntry, CUTILS_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER, CUTILS_CONTAINER_STATIC_LIST_DEFAULT_ACCESSOR, 1) \
    \
    forceinline void hash_table_type_name##HashLinkVectorCallbacker##_Expand(hash_table_type_name##HashLinkVector* arr, size_t old_capacity, size_t new_capacity) { \
        hash_table_type_name##HashLinkStaticListExpand(hash_table_type_name##HashLinkGetStaticList(arr), old_capacity, new_capacity); \
    } \
    CUTILS_CONTAINER_VECTOR_DEFINE(hash_table_type_name##HashLink, hash_table_type_name##HashLinkEntry, allocator, hash_table_type_name##HashLinkVectorCallbacker) \
    \
    static const int32_t hash_table_type_name##HashLinkRbReferencer_InvalidId = (-1) ; \
    forceinline hash_table_type_name##HashLinkRbEntry* hash_table_type_name##HashLinkRbReferencer_Reference(hash_table_type_name##HashLinkRbTree* tree, int32_t entry_id) { \
        if (entry_id == hash_table_type_name##HashLinkRbReferencer_InvalidId) { \
            return NULL; \
        } \
        hash_table_type_name##HashLinkRbObj* rb_obj = (hash_table_type_name##HashLinkRbObj*)tree; \
        return &rb_obj->table->link.obj_arr[entry_id+1].rb_entry; \
    } \
    forceinline void hash_table_type_name##HashLinkRbReferencer_Dereference(hash_table_type_name##HashLinkRbTree* tree, hash_table_type_name##HashLinkRbEntry* entry) {  } \
    \
    typedef struct { \
        int32_t color : 1; \
        int32_t parent : sizeof(int32_t) * 8 - 1; \
    } hash_table_type_name##HashLinkRbParentColor; \
    forceinline key_type hash_table_type_name##HashLinkRbAccessor##_GetKey(hash_table_type_name##HashLinkRbTree* tree, hash_table_type_name##HashLinkRbBsEntry* bs_entry) { \
        hash_table_type_name##HashLinkRbObj* rb_obj = (hash_table_type_name##HashLinkRbObj*)tree; \
        return accessor##_GetKey(rb_obj->table, ((hash_table_type_name##HashLinkEntry*)bs_entry)->obj); \
    } \
    forceinline int32_t hash_table_type_name##HashLinkRbAccessor##_GetParent(hash_table_type_name##HashLinkRbTree* tree, hash_table_type_name##HashLinkRbBsEntry* bs_entry) { \
        return (((hash_table_type_name##HashLinkRbParentColor*)&(((hash_table_type_name##HashLinkRbEntry*)bs_entry)->parent_color))->parent); \
    } \
    forceinline RbColor hash_table_type_name##HashLinkRbAccessor##_GetColor(hash_table_type_name##HashLinkRbTree* tree, hash_table_type_name##HashLinkRbBsEntry* bs_entry) { \
        return (((hash_table_type_name##HashLinkRbParentColor*)&(((hash_table_type_name##HashLinkRbEntry*)bs_entry)->parent_color))->color == -1 ? 1 : 0); \
    } \
    forceinline void hash_table_type_name##HashLinkRbAccessor##_SetParent(hash_table_type_name##HashLinkRbTree* tree, hash_table_type_name##HashLinkRbBsEntry* bs_entry, int32_t new_id) { \
        ((hash_table_type_name##HashLinkRbParentColor*)&(((hash_table_type_name##HashLinkRbEntry*)bs_entry)->parent_color))->parent = new_id; \
    } \
    forceinline void hash_table_type_name##HashLinkRbAccessor##_SetColor(hash_table_type_name##HashLinkRbTree* tree, hash_table_type_name##HashLinkRbBsEntry* bs_entry, RbColor new_color) { \
        return ((hash_table_type_name##HashLinkRbParentColor*)&(((hash_table_type_name##HashLinkRbEntry*)bs_entry)->parent_color))->color = new_color; \
    } \
    CUTILS_CONTAINER_RB_TREE_DEFINE(hash_table_type_name##HashLink, int32_t, key_type, hash_table_type_name##HashLinkRbReferencer, hash_table_type_name##HashLinkRbAccessor, comparer) \
    \
    CUTILS_CONTAINER_VECTOR_DEFINE(hash_table_type_name##HashType, int8_t, allocator, CUTILS_CONTAINER_VECTOR_DEFAULT_CALLBACKER) \
    CUTILS_CONTAINER_VECTOR_DEFINE(hash_table_type_name##HashBucket, hash_table_type_name##HashEntry, allocator, CUTILS_CONTAINER_VECTOR_DEFAULT_CALLBACKER) \
    \
    static forceinline uint32_t hash_table_type_name##HashGetIndex(hash_table_type_name##HashTable* table, const key_type* key) { \
        return hasher(table, *key) % table->bucket.capacity; \
    } \
    static forceinline uint32_t hash_table_type_name##HashGetCurrentLoadFator(hash_table_type_name##HashTable* table) { \
        return table->bucket.count * 100 / table->bucket.capacity; \
    } \
    void hash_table_type_name##HashTableInit(hash_table_type_name##HashTable* table, size_t capacity, uint32_t load_fator) { \
        if (capacity == 0) { \
            capacity = CUTILS_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE; \
        } \
        hash_table_type_name##HashBucketVectorInit(&table->bucket, capacity, true); \
        hash_table_type_name##HashTypeVectorInit(&table->type, capacity, true); \
        hash_table_type_name##HashLinkVectorInit(&table->link, capacity + 1, true); \
        hash_table_type_name##HashLinkStaticListInit(hash_table_type_name##HashLinkGetStaticList(&table->link), capacity); \
        table->bucket.count = 0; \
        \
        for (int i = 0; i < table->bucket.capacity; i++) { \
            table->type.obj_arr[i] = kHashEntryFree; \
        } \
        if (load_fator == 0) { \
            load_fator = CUTILS_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR; \
        } \
        table->load_fator = load_fator; \
    } \
    void hash_table_type_name##HashTableRelease(hash_table_type_name##HashTable* table) { \
        hash_table_type_name##HashBucketVectorRelease(&table->bucket); \
        hash_table_type_name##HashTypeVectorRelease(&table->type); \
        hash_table_type_name##HashLinkVectorRelease(&table->link); \
    } \
    element_type* hash_table_type_name##HashTableFind(hash_table_type_name##HashTable* table, const key_type* key) { \
        uint32_t index = hash_table_type_name##HashGetIndex(table, key); \
        hash_table_type_name##HashEntry* entry = &table->bucket.obj_arr[index]; \
        HashEntryType type = table->type.obj_arr[index]; \
        if (type == kHashEntryObj) { \
            if(comparer##_Equal(table, accessor##_GetKey(table, entry->obj), *key)) { \
                return &entry->obj; \
            } \
        } \
        else if (type == kHashEntryTree) { \
            int32_t rb_id = hash_table_type_name##HashLinkRbTreeFind(&entry->rb_tree, *key); \
            if(rb_id != hash_table_type_name##HashLinkRbReferencer_InvalidId) { \
                return &table->link.obj_arr[rb_id+1].obj; \
            } \
        } \
        return NULL; \
    } \
    int32_t hash_table_type_name##HashTableAllocTreeEntry(hash_table_type_name##HashTable* table) { \
        hash_table_type_name##HashLinkStaticList* static_list = hash_table_type_name##HashLinkGetStaticList(&table->link); \
        int32_t id = hash_table_type_name##HashLinkStaticListPop(static_list, 0); \
        if (id == CUTILS_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER_InvalidId) { \
        } \
        return id; \
    } \
    void hash_table_type_name##HashTableFreeTreeEntry(hash_table_type_name##HashTable* table, int32_t id) { \
        hash_table_type_name##HashLinkStaticList* static_list = hash_table_type_name##HashLinkGetStaticList(&table->link); \
        hash_table_type_name##HashLinkStaticListPush(static_list, 0, id); \
    } \
    bool hash_table_type_name##HashTablePut(hash_table_type_name##HashTable* table, const element_type* obj) { \
        key_type key = accessor##_GetKey(table, *obj); \
        uint32_t index = hash_table_type_name##HashGetIndex(table, &key); \
        hash_table_type_name##HashEntry* entry = &table->bucket.obj_arr[index]; \
        HashEntryType type = table->type.obj_arr[index]; \
        if (type == kHashEntryFree) { \
            obj_mover##_Assignment(table, entry->obj, *obj); \
            table->type.obj_arr[index] = kHashEntryObj; \
        } \
        else if (type == kHashEntryObj) { \
            if (comparer##_Equal(table, accessor##_GetKey(table, entry->obj), accessor##_GetKey(table, *obj))) { \
                obj_mover##_Assignment(table, entry->obj, *obj); \
                return true; \
            } \
            table->type.obj_arr[index] = kHashEntryTree; \
            int32_t id = hash_table_type_name##HashTableAllocTreeEntry(table); \
            obj_mover##_Assignment(table, table->link.obj_arr[id + 1], entry->obj); \
            hash_table_type_name##HashLinkRbTreeInit(&entry->rb_tree); \
            hash_table_type_name##HashLinkRbTreePut(&entry->rb_tree, id); \
            id = hash_table_type_name##HashTableAllocTreeEntry(table); \
            obj_mover##_Assignment(table, table->link.obj_arr[id + 1], *obj); \
            hash_table_type_name##HashLinkRbTreePut(&entry->rb_tree, id); \
        } \
        else if (type == kHashEntryTree) { \
            int32_t rb_id = hash_table_type_name##HashLinkRbTreeFind(&entry->rb_tree, *key); \
            if (rb_id != hash_table_type_name##HashLinkRbReferencer_InvalidId) { \
                hash_table_type_name##HashLinkRbTreeDelete(&entry->rb_tree, rb_id); \
                hash_table_type_name##HashTableFreeTreeEntry(table, rb_id); \
            } \
            rb_id = hash_table_type_name##HashTableAllocTreeEntry(table); \
            obj_mover##_Assignment(table, table->link.obj_arr[rb_id + 1], *obj); \
            hash_table_type_name##HashLinkRbTreePut(&entry->rb_tree, rb_id); \
        } \
        \
        table->bucket.count++; \
        if (hash_table_type_name##HashGetCurrentLoadFator(table) >= table->load_fator) { \
            /* 触发扩容 */ \
            /*hash_table_type_name##HashRehash(table, table->bucket.capacity * CUTILS_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR);*/ \
        } \
        return true; \
    } \
    //static void hash_table_type_name##HashRehash(hash_table_type_name##HashTable* table, size_t new_capacity) {  \
    //    hash_table_type_name##HashTable temp_table; \
    //    hash_table_type_name##HashTableInit(&temp_table, new_capacity, table->load_fator); \
    //    /* 重映射 */ \
    //    /* 优化点之一，可以添加基于迭代器删除节点的接口 */ \
    //    hash_table_type_name##HashTableIterator iter; \
    //    element_type* obj = hash_table_type_name##HashTableIteratorFirst(table, &iter); \
    //    while (obj) { \
    //        hash_table_type_name##HashTablePut(&temp_table, obj); \
    //        key_type key = accessor##_GetKey(table, *obj); \
    //        hash_table_type_name##HashTableDelete(table, &key); \
    //        obj = hash_table_type_name##HashTableIteratorNext(table, &iter); \
    //    } \
    //    \
    //    hash_table_type_name##HashBucketVectorRelease(&table->bucket); \
    //    MemoryCopy(table, &temp_table, sizeof(temp_table)); \
    //} \
    //size_t hash_table_type_name##HashTableGetCount(hash_table_type_name##HashTable* table){ return table->bucket.count; } \
    //bool hash_table_type_name##HashTableDelete(hash_table_type_name##HashTable* table, const key_type* key) { \
    //    uint32_t index = hash_table_type_name##HashGetIndex(table, key); \
    //    hash_table_type_name##HashEntry* entry = &table->bucket.obj_arr[index]; \
    //    HashEntryType type = table->type.obj_arr[index]; \
    //    bool success = true; \
    //    if (type == kHashEntryFree) { \
    //        return false; \
    //    } \
    //    else if (type == kHashEntryObj) { \
    //        if (!comparer##_Equal(table, accessor##_GetKey(table, entry->obj), *key)) { \
    //            return false; \
    //        } \
    //        table->type.obj_arr[index] = kHashEntryFree; \
    //        success = true; \
    //    } \
    //    else if (type == kHashEntryTree) { \
    //        SinglyListEntry* prev = NULL; \
    //        SinglyListEntry* cur = SinglyListIteratorFirst(&entry->list_head); \
    //        while (cur) { \
    //            if (!comparer##_Equal(table, accessor##_GetKey(table, ((hash_table_type_name##HashLinkEntry*)cur)->obj), *key)) { \
    //                prev = cur; \
    //                cur = SinglyListIteratorNext(&entry->list_head, cur); \
    //                continue; \
    //            } \
    //            if (prev) { \
    //                SinglyListDeleteEntry(&entry->list_head, prev, cur); \
    //            } \
    //            else { \
    //                SinglyListDeleteFirst(&entry->list_head); \
    //                if (SinglyListIsEmpty(&entry->list_head)) { \
    //                    table->type.obj_arr[index] = kHashEntryFree; \
    //                } \
    //            } \
    //            allocator##_Release(table, cur); \
    //            success = true; \
    //            break; \
    //        } \
    //    } \
    //    if (success) table->bucket.count--; \
    //    return success; \
    //} \
    ///* 
    //* 现在的迭代思路是遍历空间所有节点，另外可以用静态链表连接所有已映射的节点，但需要额外空间
    //*/ \
    //element_type* hash_table_type_name##HashTableIteratorFirst(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
    //    iter->cur_list_entry = NULL; \
    //    iter->cur_index = 0; \
    //    CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_1 \
    //    return hash_table_type_name##HashTableIteratorNext(table, iter); \
    //} \
    //element_type* hash_table_type_name##HashTableIteratorNext(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
    //    if (iter->cur_list_entry) { \
    //        hash_table_type_name##HashLinkEntry* cur = iter->cur_list_entry; \
    //        hash_table_type_name##HashEntry* entry = &table->bucket.obj_arr[iter->cur_index]; \
    //        iter->cur_list_entry = (hash_table_type_name##HashLinkEntry*)SinglyListIteratorNext(&entry->list_head, &cur->list_entry); \
    //        if (iter->cur_list_entry == NULL) { \
    //            iter->cur_index++; \
    //        } \
    //        if (cur) { \
    //            CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_2 \
    //            return &cur->obj; \
    //        } \
    //    } \
    //    for (; iter->cur_index < table->bucket.capacity; iter->cur_index++) { \
    //        hash_table_type_name##HashEntry* entry = &table->bucket.obj_arr[iter->cur_index]; \
    //        HashEntryType type = table->type.obj_arr[iter->cur_index]; \
    //        if (type == kHashEntryFree) { \
    //            CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_3 \
    //            continue; \
    //        } \
    //        if (type == kHashEntryObj) { \
    //            CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_4 \
    //            iter->cur_index++; \
    //            return &entry->obj; \
    //        } \
    //        if (type == kHashEntryTree) { \
    //            CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_5 \
    //            iter->cur_list_entry = (hash_table_type_name##HashLinkEntry*)entry->list_head.first; \
    //            return hash_table_type_name##HashTableIteratorNext(table, iter); \
    //        } \
    //    } \
    //    return NULL; \
    //} \
 


//CUTILS_CONTAINER_HASH_TABLE_DECLARATION(Int, int, int)
//#define INT_HASHTABLE_ACCESSOR_GetKey(obj) (obj)
//#define INT_HASHTABLE_ACCESSOR INT_HASHTABLE_ACCESSOR
//#define INT_HASHTABLE_HASHER_KEY(key) Hashmap_hashint(key)
//CUTILS_CONTAINER_HASH_TABLE_DEFINE(Int, int, int, CUTILS_OBJECT_ALLOCATOR_DEFALUT, INT_HASHTABLE_ACCESSOR, CUTILS_OBJECT_MOVER_DEFALUT, INT_HASHTABLE_HASHER_KEY, CUTILS_OBJECT_COMPARER_DEFALUT)

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_HASH_TABLE_H_