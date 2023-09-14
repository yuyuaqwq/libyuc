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
* 链出单向链表的哈希表实现
*/
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE 16
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR 75//%
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR 2



#define LIBYUC_CONTAINER_HASH_TABLE_DECLARATION(hash_table_type_name, count_type, element_type, key_type)
    typedef struct HashTableIterator{
        count_type cur_index;
        count_type entry_prev_id;
        count_type entry_cur_id;
        LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DECLARATION
    } HashTableIterator;
   
    element_type* HashTableIteratorFirst(struct _##HashTable* table, HashTableIterator* iter);
    element_type* HashTableIteratorNext(struct _##HashTable* table, HashTableIterator* iter);
    element_type* HashTableIteratorLocate(struct _##HashTable* table, HashTableIterator* iter, const key_type* key);
    void HashTableIteratorPut(struct _##HashTable* table, HashTableIterator* iter, const element_type* obj);
    bool HashTableIteratorDelete(struct _##HashTable* table, HashTableIterator* iter);
   
   
    LIBYUC_CONTAINER_SINGLY_LIST_DECLARATION(
        HashLink,
        count_type,
        count_type
        )
    typedef struct _##HashLinkEntry {
        union {
            count_type next;
            HashLinkSinglyListEntry entry;
        };
        element_type obj;
    } HashLinkEntry;
    typedef struct _##HashLinkMainObj { /* 传递时的主对象 */
        HashLinkSinglyListHead head;
        struct _##HashTable* table;
    } HashLinkMainObj;
   
    LIBYUC_CONTAINER_STATIC_LIST_DECLARATION(
        HashLink,
        count_type,
        HashLinkEntry,
        1
        )
    LIBYUC_CONTAINER_VECTOR_DECLARATION(
        HashLink,
        LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC,
        count_type,
        count_type,
        HashLinkEntry
        )
   
    LIBYUC_CONTAINER_VECTOR_DECLARATION(
        HashBucket,
        LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC,
        count_type,
        count_type,
        struct _##HashTableEntry
        )
   
    typedef struct _##HashTableEntry {
        union {
            HashLinkSinglyListHead list_head;
        };
    } HashTableEntry;
    typedef struct _##HashTable {
        HashBucketVector bucket;
        HashLinkVector link;
        count_type load_fator;
        count_type mask;
    } ##HashTable;
   
    void HashTableInit(HashTable* table, count_type capacity, count_type load_fator);
    void HashTableRelease(HashTable* table);
    count_type HashTableGetCount(HashTable* table);
    element_type* HashTableFind(HashTable* table, const key_type* key);
    void HashTablePut(HashTable* table, const element_type* obj);
    bool HashTableDelete(HashTable* table, const key_type* key);



// 访问器需要提供_GetKey方法
    /*
    * 静态链表
    */
    static HashLinkStaticList* HashLinkGetStaticList(HashLinkVector* link_vector) {
        return (HashLinkStaticList*)((uintptr_t)&link_vector->obj_arr[1] - sizeof(HashLinkStaticList));
    }
    LIBYUC_CONTAINER_STATIC_LIST_DEFINE(
        HashLink,
        count_type,
        HashLinkEntry,
        LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_ACCESSOR,
        LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER,
        1
        )
   
    static const count_type HashLinkReferencer_InvalidId = (-1);
    forceinline HashLinkSinglyListEntry* HashLinkReferencer_Reference(HashLinkSinglyListHead* head, count_type entry_id) {
        if (entry_id == HashLinkReferencer_InvalidId) return NULL;
        HashLinkMainObj* obj = (HashLinkMainObj*)head;
        return &obj->table->link.obj_arr[entry_id + 1].entry;
    }
    forceinline void HashLinkReferencer_Dereference(HashLinkSinglyListHead* head, HashLinkSinglyListEntry* entry) {    }
    LIBYUC_CONTAINER_SINGLY_LIST_DEFINE(
        HashLink,
        count_type,
        count_type,
        HashLinkReferencer
        )
    /*
    * 动态数组
    */
    forceinline void HashLinkVectorCallbacker##_Expand(HashLinkVector* arr, size_t old_capacity, size_t new_capacity) {
        HashLinkStaticListExpand(HashLinkGetStaticList(arr), old_capacity, new_capacity);
    }
    LIBYUC_CONTAINER_VECTOR_DEFINE(
        HashLink,
        LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC,
        count_type,
        count_type,
        HashLinkEntry,
        HashLinkEntry,
        LIBYUC_CONTAINER_VECTOR_ACCESSOR_DEFAULT,
        allocator,
        HashLinkVectorCallbacker,
        LIBYUC_CONTAINER_VECTOR_COMPARER_INVALID,
        LIBYUC_CONTAINER_VECTOR_REFERENCER_DEFALUT
        )
    LIBYUC_CONTAINER_VECTOR_DEFINE(
        HashBucket,
        LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC,
        count_type,
        count_type,
        HashTableEntry,
        HashTableEntry,
        LIBYUC_CONTAINER_VECTOR_ACCESSOR_DEFAULT,
        allocator,
        LIBYUC_CONTAINER_VECTOR_CALLBACKER_DEFAULT,
        LIBYUC_CONTAINER_VECTOR_COMPARER_INVALID,
        LIBYUC_CONTAINER_VECTOR_REFERENCER_DEFALUT
        )
   
    /*
    * 哈希表
    */
    static forceinline count_type HashModIndex(HashTable* table, count_type index) {
        return index & table->mask;
    }
    static forceinline count_type HashGetIndex(HashTable* table, const key_type* key) {
        return HashModIndex(table, hasher(table, key));
    }
    static forceinline count_type HashGetCurrentLoadFator(HashTable* table) {
        return HashBucketVectorGetCount(&table->bucket) * 100 / HashBucketVectorGetCapacity(&table->bucket);
    }
    static count_type HashTableAllocLinkEntry(HashTable* table) {
        HashLinkStaticList* static_list = HashLinkGetStaticList(&table->link);
        count_type id = HashLinkStaticListPop(static_list, 0);
         assert(id != LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER_InvalidId);
        return id;
    }
    static void HashTableFreeLinkEntry(HashTable* table, count_type id) {
        HashLinkStaticList* static_list = HashLinkGetStaticList(&table->link);
        HashLinkStaticListPush(static_list, 0, id);
    }
    count_type HashTableGetCount(HashTable* table){
        return HashBucketVectorGetCount(&table->bucket);
    }
    /* 重映射 */
    static void HashRehash(HashTable* table, count_type new_capacity) {   
        HashTable temp_table;
        HashTableInit(&temp_table, new_capacity, table->load_fator);
        HashTableIterator iter;
        element_type* obj = HashTableIteratorFirst(table, &iter);
        while (obj) {
            HashTablePut(&temp_table, obj);
            key_type* key = accessor##_GetKey(table, obj);
            obj = HashTableIteratorNext(table, &iter);
        }
        HashTableRelease(table);
        MemoryCopy(table, &temp_table, sizeof(temp_table));
    }
    void HashTableInit(HashTable* table, count_type capacity, count_type load_fator) {
        if (capacity < 2) {
            capacity = LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE;
        }
        HashBucketVectorInit(&table->bucket, capacity);
        HashLinkVectorInit(&table->link, capacity + 1/* 预留StaticListHead的位置 */);
        HashLinkStaticListInit(HashLinkGetStaticList(&table->link), capacity);
       
        for (count_type i = 0; i < HashBucketVectorGetCapacity(&table->bucket); i++) {
            HashLinkSinglyListHeadInit(&HashBucketVectorIndex(&table->bucket, i)->list_head);
        }
        if (load_fator == 0) {
            load_fator = LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR;
        }
        table->mask = 0;
        while(capacity /= 2) {;
            table->mask = (table->mask << 1) | 1;
        }
        table->load_fator = load_fator;
    }
    void HashTableRelease(HashTable* table) {
        HashBucketVectorRelease(&table->bucket);
        HashLinkVectorRelease(&table->link);
    }
   
    forceinline element_type* HashTableIteratorLocate(HashTable* table, HashTableIterator* iter, const key_type* key) {
        count_type index = HashGetIndex(table, key);
        iter->cur_index = index;
        HashTableEntry* entry = HashBucketVectorIndex(&table->bucket, index);
        HashLinkMainObj main_obj;
        main_obj.head = entry->list_head;
        main_obj.table = table;
        iter->entry_cur_id = HashLinkSinglyListIteratorFirst(&main_obj.head);
        iter->entry_prev_id = HashLinkReferencer_InvalidId;
        while (iter->entry_cur_id != HashLinkReferencer_InvalidId) {
            HashLinkEntry* link_entry = HashLinkVectorIndex(&table->link, iter->entry_cur_id + 1);
            if (comparer##_Equal(table, accessor##_GetKey(table, &link_entry->obj), key)) {
                return &link_entry->obj;
            }
            iter->entry_prev_id = iter->entry_cur_id;
            iter->entry_cur_id = HashLinkSinglyListIteratorNext(&main_obj.head, iter->entry_cur_id);
        }
        return NULL;
    }
    void HashTableIteratorPut(HashTable* table, HashTableIterator* iter, const element_type* obj) {
        HashTableEntry* entry = HashBucketVectorIndex(&table->bucket, iter->cur_index);
        HashLinkMainObj main_obj;
        main_obj.table = table;
        main_obj.head = entry->list_head;
        if (iter->entry_cur_id != HashLinkReferencer_InvalidId) {
            HashLinkEntry* link_entry = (HashLinkEntry*)HashLinkReferencer_Reference(&main_obj.head, iter->entry_cur_id);
            if (comparer##_Equal(table, accessor##_GetKey(table, &link_entry->obj), accessor##_GetKey(table, obj))) {
                obj_mover##_Copy(table, &link_entry->obj, obj);
                return;
            }
            HashLinkReferencer_Dereference(&main_obj.head, link_entry);
        }
        count_type link_entry_id = HashTableAllocLinkEntry(table);
        obj_mover##_Copy(table, &HashLinkVectorIndex(&table->link, link_entry_id + 1)->obj, obj);
        HashLinkSinglyListPutFirst(&main_obj.head, link_entry_id);
        HashBucketVectorSetCount(&table->bucket, HashBucketVectorGetCount(&table->bucket) + 1);
        entry->list_head = main_obj.head;
       
        if (HashGetCurrentLoadFator(table) >= table->load_fator) {
            /* 触发扩容 */
            HashRehash(table, HashBucketVectorGetCapacity(&table->bucket) * LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR);
        }
    }
    bool HashTableIteratorDelete(HashTable* table, HashTableIterator* iter) {
        HashTableEntry* entry = HashBucketVectorIndex(&table->bucket, iter->cur_index);
        HashLinkMainObj main_obj;
        main_obj.head = entry->list_head;
        main_obj.table = table;
        count_type link_entry_id = iter->entry_cur_id;
        if (link_entry_id == HashLinkReferencer_InvalidId) return false;
        HashLinkSinglyListDeleteEntry(&main_obj.head, iter->entry_prev_id, link_entry_id);
        element_type* old_element = &HashLinkVectorIndex(&table->link, link_entry_id + 1)->obj;
        HashBucketVectorSetCount(&table->bucket, HashBucketVectorGetCount(&table->bucket) - 1);
        entry->list_head = main_obj.head;
        HashTableFreeLinkEntry(table, link_entry_id);
        if (HashBucketVectorGetCapacity(&table->bucket) > LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE && HashGetCurrentLoadFator(table) <= 100 - table->load_fator) {
            HashRehash(table, HashBucketVectorGetCapacity(&table->bucket) / LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR);
        }
        return true;
    }
    element_type* HashTableIteratorFirst(HashTable* table, HashTableIterator* iter) {
        iter->cur_index = 0;
        iter->entry_cur_id = HashLinkReferencer_InvalidId;
        LIBYUC_CONTAINER_HASH_TABLE_DATA_STATISTICS_DEFINE_1
        return HashTableIteratorNext(table, iter);
    }
    element_type* HashTableIteratorNext(HashTable* table, HashTableIterator* iter) {
        HashLinkMainObj main_obj;
        main_obj.table = table;
        do {
            if (iter->cur_index >= HashBucketVectorGetCapacity(&table->bucket)) {
                return NULL;
            }
            if (iter->entry_cur_id == HashLinkReferencer_InvalidId) {
                main_obj.head = HashBucketVectorIndex(&table->bucket, iter->cur_index)->list_head;
                iter->entry_cur_id = HashLinkSinglyListIteratorFirst(&main_obj.head);
                iter->entry_prev_id = HashLinkReferencer_InvalidId;
            } else {
                main_obj.head = HashBucketVectorIndex(&table->bucket, iter->cur_index)->list_head;
                iter->entry_prev_id = iter->entry_cur_id;
                iter->entry_cur_id = HashLinkSinglyListIteratorNext(&main_obj.head, iter->entry_cur_id);
            }
            if (iter->entry_cur_id == HashLinkReferencer_InvalidId) {
                ++iter->cur_index;
                continue;
            }
            break;
        } while (true);
        element_type* element = &HashLinkVectorIndex(&table->link, iter->entry_cur_id + 1)->obj;
        return element;
    }
   
    element_type* HashTableFind(HashTable* table, const key_type* key) {
        HashTableIterator iter;
        return HashTableIteratorLocate(table, &iter, key);
    }
    void HashTablePut(HashTable* table, const element_type* obj) {
        key_type* key = accessor##_GetKey(table, obj);
        HashTableIterator iter;
        HashTableIteratorLocate(table, &iter, key);
        HashTableIteratorPut(table, &iter, obj);
    }
    bool HashTableDelete(HashTable* table, const key_type* key) {
        HashTableIterator iter;
        HashTableIteratorLocate(table, &iter, key);
        return HashTableIteratorDelete(table, &iter);
    }
   


#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR_GetKey(MAIN_OBJ, OBJ) (OBJ)
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_HASHER(TABLE, KEY) HashCode_hashint(KEY)


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_HASH_TABLE_H_