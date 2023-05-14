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


    typedef enum _HashTableEntryType {
        kHashTableEntryFree,
        kHashTableEntryObj,
        kHashTableEntryTree,
    } HashTableEntryType;

#define CUTILS_CONTAINER_HASH_TABLE_DECLARATION(hash_table_type_name, element_type, key_type) \
    typedef struct _##hash_table_type_name##HashTableIterator{ \
        uint32_t cur_index; \
        int32_t rb_cur_id; \
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
    CUTILS_CONTAINER_VECTOR_DECLARATION(hash_table_type_name##HashBucket, struct _##hash_table_type_name##HashTableEntry) \
    \
    typedef struct _##hash_table_type_name##HashTableEntry { \
        hash_table_type_name##HashLinkRbTree rb_tree; \
    } hash_table_type_name##HashTableEntry; \
    typedef struct _##hash_table_type_name##HashTable { \
        hash_table_type_name##HashBucketVector bucket; \
        hash_table_type_name##HashLinkVector link; \
        uint32_t load_fator; \
    } ##hash_table_type_name##HashTable; \
    \
    void hash_table_type_name##HashTableInit(hash_table_type_name##HashTable* table, size_t capacity, uint32_t load_fator); \
    size_t hash_table_type_name##HashTableGetCount(hash_table_type_name##HashTable* table); \
    element_type* hash_table_type_name##HashTableFind(hash_table_type_name##HashTable* table, const key_type* key); \
    element_type* hash_table_type_name##HashTablePut(hash_table_type_name##HashTable* table, const element_type* obj); \
    element_type* hash_table_type_name##HashTableDelete(hash_table_type_name##HashTable* table, const key_type* key); \



// 访问器需要提供_GetKey方法
#define CUTILS_CONTAINER_HASH_TABLE_DEFINE(hash_table_type_name, element_type, key_type, allocator, accessor, obj_mover, hasher, comparer) \
    /*
    * 静态链表
    */ \
    static hash_table_type_name##HashLinkStaticList* hash_table_type_name##HashLinkGetStaticList(hash_table_type_name##HashLinkVector* link_vector) { \
        return (hash_table_type_name##HashLinkStaticList*)((uintptr_t)&link_vector->obj_arr[1] - sizeof(hash_table_type_name##HashLinkStaticList)); \
    } \
    CUTILS_CONTAINER_STATIC_LIST_DEFINE(hash_table_type_name##HashLink, int32_t, hash_table_type_name##HashLinkEntry, CUTILS_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER, CUTILS_CONTAINER_STATIC_LIST_DEFAULT_ACCESSOR, 1) \
    /*
    * 动态数组
    */ \
    forceinline void hash_table_type_name##HashLinkVectorCallbacker##_Expand(hash_table_type_name##HashLinkVector* arr, size_t old_capacity, size_t new_capacity) { \
        hash_table_type_name##HashLinkStaticListExpand(hash_table_type_name##HashLinkGetStaticList(arr), old_capacity, new_capacity); \
    } \
    CUTILS_CONTAINER_VECTOR_DEFINE(hash_table_type_name##HashLink, hash_table_type_name##HashLinkEntry, allocator, hash_table_type_name##HashLinkVectorCallbacker) \
    CUTILS_CONTAINER_VECTOR_DEFINE(hash_table_type_name##HashBucket, hash_table_type_name##HashTableEntry, allocator, CUTILS_CONTAINER_VECTOR_DEFAULT_CALLBACKER) \
    /*
    * 红黑树
    */ \
    static const int32_t hash_table_type_name##HashLinkRbReferencer_InvalidId = (-1) ; \
    forceinline hash_table_type_name##HashLinkRbEntry* hash_table_type_name##HashLinkRbReferencer_Reference(hash_table_type_name##HashLinkRbTree* tree, int32_t entry_id) { \
        if (entry_id == hash_table_type_name##HashLinkRbReferencer_InvalidId) return NULL; \
        hash_table_type_name##HashLinkRbObj* rb_obj = (hash_table_type_name##HashLinkRbObj*)tree; \
        return &rb_obj->table->link.obj_arr[entry_id+1].rb_entry; \
    } \
    forceinline void hash_table_type_name##HashLinkRbReferencer_Dereference(hash_table_type_name##HashLinkRbTree* tree, hash_table_type_name##HashLinkRbEntry* entry) {  } \
    \
    typedef struct { \
        int32_t color : 1; \
        int32_t parent : sizeof(int32_t) * 8 - 1; \
    } hash_table_type_name##HashLinkRbParentColor; \
    forceinline key_type* hash_table_type_name##HashLinkRbAccessor##_GetKey(hash_table_type_name##HashLinkRbTree* tree, hash_table_type_name##HashLinkRbBsEntry* bs_entry) { \
        hash_table_type_name##HashLinkRbObj* rb_obj = (hash_table_type_name##HashLinkRbObj*)tree; \
        return accessor##_GetKey(rb_obj->table, &((hash_table_type_name##HashLinkEntry*)bs_entry)->obj); \
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
    /*
    * 哈希表
    */ \
    static forceinline uint32_t hash_table_type_name##HashGetIndex(hash_table_type_name##HashTable* table, const key_type* key) { \
        return hasher(table, key) % table->bucket.capacity; \
    } \
    static forceinline uint32_t hash_table_type_name##HashGetCurrentLoadFator(hash_table_type_name##HashTable* table) { \
        return table->bucket.count * 100 / table->bucket.capacity; \
    } \
    static int32_t hash_table_type_name##HashTableAllocTreeEntry(hash_table_type_name##HashTable* table) { \
        hash_table_type_name##HashLinkStaticList* static_list = hash_table_type_name##HashLinkGetStaticList(&table->link); \
        int32_t id = hash_table_type_name##HashLinkStaticListPop(static_list, 0); \
          assert(id != CUTILS_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER_InvalidId); \
        return id; \
    } \
    static void hash_table_type_name##HashTableFreeTreeEntry(hash_table_type_name##HashTable* table, int32_t id) { \
        hash_table_type_name##HashLinkStaticList* static_list = hash_table_type_name##HashLinkGetStaticList(&table->link); \
        hash_table_type_name##HashLinkStaticListPush(static_list, 0, id); \
    } \
    size_t hash_table_type_name##HashTableGetCount(hash_table_type_name##HashTable* table){ return table->bucket.count; } \
    /* 重映射 */ \
    static void hash_table_type_name##HashRehash(hash_table_type_name##HashTable* table, size_t new_capacity) {  \
        hash_table_type_name##HashTable temp_table; \
        hash_table_type_name##HashTableInit(&temp_table, new_capacity, table->load_fator); \
        hash_table_type_name##HashTableIterator iter; \
        element_type* obj = hash_table_type_name##HashTableIteratorFirst(table, &iter); \
        while (obj) { \
            hash_table_type_name##HashTablePut(&temp_table, obj); \
            key_type* key = accessor##_GetKey(table, obj); \
            obj = hash_table_type_name##HashTableIteratorNext(table, &iter); \
        } \
        hash_table_type_name##HashBucketVectorRelease(&table->bucket); \
        MemoryCopy(table, &temp_table, sizeof(temp_table)); \
    } \
    void hash_table_type_name##HashTableInit(hash_table_type_name##HashTable* table, size_t capacity, uint32_t load_fator) { \
        if (capacity == 0) { \
            capacity = CUTILS_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE; \
        } \
        hash_table_type_name##HashBucketVectorInit(&table->bucket, capacity, true); \
        hash_table_type_name##HashLinkVectorInit(&table->link, capacity + 1, true); \
        hash_table_type_name##HashLinkStaticListInit(hash_table_type_name##HashLinkGetStaticList(&table->link), capacity); \
        table->bucket.count = 0; \
        \
        for (int i = 0; i < table->bucket.capacity; i++) { \
            hash_table_type_name##HashLinkRbTreeInit(&table->bucket.obj_arr[i].rb_tree); \
        } \
        if (load_fator == 0) { \
            load_fator = CUTILS_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR; \
        } \
        table->load_fator = load_fator; \
    } \
    void hash_table_type_name##HashTableRelease(hash_table_type_name##HashTable* table) { \
        hash_table_type_name##HashBucketVectorRelease(&table->bucket); \
        hash_table_type_name##HashLinkVectorRelease(&table->link); \
    } \
    element_type* hash_table_type_name##HashTableFind(hash_table_type_name##HashTable* table, const key_type* key) { \
        hash_table_type_name##HashTableEntry* entry = &table->bucket.obj_arr[hash_table_type_name##HashGetIndex(table, key)]; \
        hash_table_type_name##HashLinkRbObj rb_obj; \
		rb_obj.rb_tree = entry->rb_tree; \
		rb_obj.table = table; \
        int32_t rb_id = hash_table_type_name##HashLinkRbTreeFind(&rb_obj.rb_tree, key); \
        if(rb_id != hash_table_type_name##HashLinkRbReferencer_InvalidId) { \
            return &table->link.obj_arr[rb_id+1].obj; \
        } \
        return NULL; \
    } \
    /*
    * 返回被覆盖的element
    * 调用后返回值仅在临时有效(在调用其他接口前进行拷贝)
    */ \
    element_type* hash_table_type_name##HashTablePut(hash_table_type_name##HashTable* table, const element_type* obj) { \
        key_type* key = accessor##_GetKey(table, obj); \
        hash_table_type_name##HashTableEntry* entry = &table->bucket.obj_arr[hash_table_type_name##HashGetIndex(table, key)]; \
        hash_table_type_name##HashLinkRbObj rb_obj; \
		rb_obj.rb_tree = entry->rb_tree; \
		rb_obj.table = table; \
        int32_t rb_id = hash_table_type_name##HashTableAllocTreeEntry(table); \
        obj_mover##_Assignment(table, &table->link.obj_arr[rb_id + 1].obj, obj); \
        int32_t old_rb_id = hash_table_type_name##HashLinkRbTreePut(&rb_obj.rb_tree, rb_id); \
        element_type* old_element = NULL; \
        if (old_rb_id != hash_table_type_name##HashLinkRbReferencer_InvalidId) { \
            old_element = &table->link.obj_arr[old_rb_id + 1].obj; \
            hash_table_type_name##HashTableFreeTreeEntry(table, old_rb_id); \
        } \
        else { \
            table->bucket.count++; \
        } \
        entry->rb_tree = rb_obj.rb_tree; \
        \
        if (hash_table_type_name##HashGetCurrentLoadFator(table) >= table->load_fator) { \
            /* 触发扩容 */ \
            hash_table_type_name##HashRehash(table, table->bucket.capacity * CUTILS_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR); \
        } \
        return old_element; \
    } \
    element_type* hash_table_type_name##HashTableDelete(hash_table_type_name##HashTable* table, const key_type* key) { \
        hash_table_type_name##HashTableEntry* entry = &table->bucket.obj_arr[hash_table_type_name##HashGetIndex(table, key)]; \
        hash_table_type_name##HashLinkRbObj rb_obj; \
		rb_obj.rb_tree = entry->rb_tree; \
		rb_obj.table = table; \
        int32_t rb_id = hash_table_type_name##HashLinkRbTreeFind(&rb_obj.rb_tree, key); \
        if (rb_id == hash_table_type_name##HashLinkRbReferencer_InvalidId) return false; \
        bool success = hash_table_type_name##HashLinkRbTreeDelete(&rb_obj.rb_tree, rb_id); \
        if (success) { \
            element_type* old_element = &table->link.obj_arr[rb_id + 1].obj; \
            hash_table_type_name##HashTableFreeTreeEntry(table, rb_id); \
            table->bucket.count--; \
            entry->rb_tree = rb_obj.rb_tree; \
            return old_element; \
        }\
        return NULL; \
    } \
    element_type* hash_table_type_name##HashTableIteratorFirst(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
        iter->cur_index = -1; \
        iter->rb_cur_id = hash_table_type_name##HashLinkRbReferencer_InvalidId; \
        CUTILS_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_1 \
        return hash_table_type_name##HashTableIteratorNext(table, iter); \
    } \
    element_type* hash_table_type_name##HashTableIteratorNext(hash_table_type_name##HashTable* table, hash_table_type_name##HashTableIterator* iter) { \
        hash_table_type_name##HashLinkRbObj rb_obj; \
		rb_obj.table = table; \
        do { \
            if (iter->rb_cur_id == hash_table_type_name##HashLinkRbReferencer_InvalidId) { \
                if ((int32_t)iter->cur_index >= (int32_t)table->bucket.capacity - 1) { \
                    break; \
                } \
                rb_obj.rb_tree = table->bucket.obj_arr[++iter->cur_index].rb_tree; \
                iter->rb_cur_id = hash_table_type_name##HashLinkRbTreeIteratorFirst(&rb_obj.rb_tree); \
            } else { \
                rb_obj.rb_tree = table->bucket.obj_arr[iter->cur_index].rb_tree; \
                iter->rb_cur_id = hash_table_type_name##HashLinkRbTreeIteratorNext(&rb_obj.rb_tree, iter->rb_cur_id); \
            }\
        } while (iter->rb_cur_id == hash_table_type_name##HashLinkRbReferencer_InvalidId); \
        if (iter->rb_cur_id == hash_table_type_name##HashLinkRbReferencer_InvalidId) return NULL; \
        hash_table_type_name##HashLinkRbEntry* entry = hash_table_type_name##HashLinkRbReferencer_Reference(&rb_obj.rb_tree, iter->rb_cur_id); \
        return &((hash_table_type_name##HashLinkEntry*)entry)->obj; \
    } \


#define CUTILS_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR_GetKey(OBJ) (OBJ)
#define CUTILS_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR CUTILS_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR
#define CUTILS_CONTAINER_HASH_TABLE_DEFAULT_HASHER(TABLE, KEY) Hashmap_hashint(KEY)


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_HASH_TABLE_H_