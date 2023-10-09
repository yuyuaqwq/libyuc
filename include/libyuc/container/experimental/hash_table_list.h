/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>
#include <libyuc/algorithm/hash_code.h>


/*
* 链出单向链表的哈希表实现
*/


#ifndef LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME
#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_ACCESSOR_GetKey
#define LIBYUC_CONTAINER_HASH_TABLE_ACCESSOR_GetKey(main_obj, obj) (obj)
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element int
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Const_InvalidId
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Const_InvalidId (-1)
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id size_t
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Offset
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Offset LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode
#define LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode(main_obj, obj) HashCode_hashint(*obj)
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_COMPARER_Type_Diff
#define LIBYUC_CONTAINER_HASH_TABLE_COMPARER_Type_Diff ptrdiff_t
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_COMPARER_Cmp
#define LIBYUC_CONTAINER_HASH_TABLE_COMPARER_Cmp(main_obj, obj1, obj2) (LIBYUC_CONTAINER_HASH_TABLE_COMPARER_Type_Diff)((*(obj1) - *(obj2)))
#endif


#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE 16
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR 75//%
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR 2


typedef struct HashTableIterator{
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id cur_index;
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id entry_prev_id;
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id entry_cur_id;
} HashTableIterator;
   

#define LIBYUC_CONTAINER_SINGLY_LIST_CLASS_NAME HashLink
#define LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Type_Id size_t
struct HashLinkSinglyListEntry* HashLinkReferencer_Reference(struct HashLinkSinglyListHead* head, LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id entry_id);
struct HashLinkSinglyListEntry* HashLinkReferencer_Reference(struct HashLinkSinglyListHead* head, LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id entry_id);
#define LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Reference HashLinkReferencer_Reference
#define LIBYUC_CONTAINER_SINGLY_LIST_REFERENCER_Defeference HashLinkReferencer_Dereference
#include <libyuc/container/singly_list.h>


#define LIBYUC_CONTAINER_VECTOR_CLASS_NAME HashBucket
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element struct HashTableEntry
#include <libyuc/container/vector.h>


#define LIBYUC_CONTAINER_VECTOR_CLASS_NAME HashLink
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element struct HashLinkEntry
void HashLinkVectorExpandCallbacker(struct HashLinkVector* arr, size_t old_capacity, size_t new_capacity);
#define LIBYUC_CONTAINER_VECTOR_CALLBACK_Expand HashLinkVectorExpandCallbacker
#include <libyuc/container/vector.h>


#define LIBYUC_CONTAINER_STATIC_LIST_CLASS_NAME HashLink
#include <libyuc/container/static_list.h>


typedef struct HashLinkMainObj{ /* 传递时的主对象 */
    HashLinkSinglyListHead head;
    struct HashTable* table;
} HashLinkMainObj;

typedef struct HashLinkEntry {
    union {
        LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id next;
        HashLinkSinglyListEntry entry;
    };
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element obj;
} HashLinkEntry;

typedef struct HashTableEntry{
union {
    HashLinkSinglyListHead list_head;
};
} HashTableEntry;
typedef struct HashTable{
    HashBucketVector bucket;
    HashLinkVector link;
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id load_fator;
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id mask;
} HashTable;

LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* HashTableIteratorFirst(HashTable* table, HashTableIterator* iter);
LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* HashTableIteratorNext(HashTable* table, HashTableIterator* iter);
LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* HashTableIteratorLocate(HashTable* table, HashTableIterator* iter, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key* key);
void HashTableIteratorPut(HashTable* table, HashTableIterator* iter, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* obj);
bool HashTableIteratorDelete(HashTable* table, HashTableIterator* iter);
   
   

   
void HashTableInit(HashTable* table, LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id capacity, LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id load_fator);
void HashTableRelease(HashTable* table);
LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id HashTableGetCount(HashTable* table);
LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* HashTableFind(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key* key);
void HashTablePut(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* obj);
bool HashTableDelete(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key* key);



/*
* 静态链表
*/
static HashLinkStaticList* HashLinkGetStaticList(HashLinkVector* link_vector) {
    return (HashLinkStaticList*)((uintptr_t)&link_vector->obj_arr[1] - sizeof(HashLinkStaticList));
}

static const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id HashLinkReferencer_InvalidId = (-1);
forceinline HashLinkSinglyListEntry* HashLinkReferencer_Reference(HashLinkSinglyListHead* head, LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id entry_id) {
    if (entry_id == HashLinkReferencer_InvalidId) return NULL;
    HashLinkMainObj* obj = (HashLinkMainObj*)head;
    return &obj->table->link.obj_arr[entry_id + 1].entry;
}
forceinline void HashLinkReferencer_Dereference(HashLinkSinglyListHead* head, HashLinkSinglyListEntry* entry) {    }

/*
* 动态数组
*/
forceinline void HashLinkVectorExpandCallbacker(HashLinkVector* arr, size_t old_capacity, size_t new_capacity) {
    HashLinkStaticListExpand(HashLinkGetStaticList(arr), old_capacity, new_capacity);
}

/*
* 哈希表
*/
static forceinline LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id HashModIndex(HashTable* table, LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id index) {
    return index & table->mask;
}
static forceinline LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id HashGetIndex(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key* key) {
    return HashModIndex(table, LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode(table, key));
}
static forceinline LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id HashGetCurrentLoadFator(HashTable* table) {
    return HashBucketVectorGetCount(&table->bucket) * 100 / HashBucketVectorGetCapacity(&table->bucket);
}
static LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id HashTableAllocLinkEntry(HashTable* table) {
    HashLinkStaticList* static_list = HashLinkGetStaticList(&table->link);
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id id = HashLinkStaticListPop(static_list, 0);
        assert(id != LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Const_InvalidId);
    return id;
}
static void HashTableFreeLinkEntry(HashTable* table, LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id id) {
    HashLinkStaticList* static_list = HashLinkGetStaticList(&table->link);
    HashLinkStaticListPush(static_list, 0, id);
}
LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id HashTableGetCount(HashTable* table){
    return HashBucketVectorGetCount(&table->bucket);
}
/* 重映射 */
static void HashRehash(HashTable* table, LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id new_capacity) {   
    HashTable temp_table;
    HashTableInit(&temp_table, new_capacity, table->load_fator);
    HashTableIterator iter;
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* obj = HashTableIteratorFirst(table, &iter);
    while (obj) {
        HashTablePut(&temp_table, obj);
        LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key* key = LIBYUC_CONTAINER_HASH_TABLE_ACCESSOR_GetKey(table, obj);
        obj = HashTableIteratorNext(table, &iter);
    }
    HashTableRelease(table);
    MemoryCopy(table, &temp_table, sizeof(temp_table));
}
void HashTableInit(HashTable* table, LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id capacity, LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id load_fator) {
    if (capacity < 2) {
        capacity = LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE;
    }
    HashBucketVectorInit(&table->bucket, capacity);
    HashLinkVectorInit(&table->link, capacity + 1/* 预留StaticListHead的位置 */);
    HashLinkStaticListInit(HashLinkGetStaticList(&table->link), capacity);
       
    for (LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id i = 0; i < HashBucketVectorGetCapacity(&table->bucket); i++) {
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
   
forceinline LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* HashTableIteratorLocate(HashTable* table, HashTableIterator* iter, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key* key) {
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id index = HashGetIndex(table, key);
    iter->cur_index = index;
    HashTableEntry* entry = HashBucketVectorIndex(&table->bucket, index);
    HashLinkMainObj main_obj;
    main_obj.head = entry->list_head;
    main_obj.table = table;
    iter->entry_cur_id = HashLinkSinglyListIteratorFirst(&main_obj.head);
    iter->entry_prev_id = HashLinkReferencer_InvalidId;
    while (iter->entry_cur_id != HashLinkReferencer_InvalidId) {
        HashLinkEntry* link_entry = HashLinkVectorIndex(&table->link, iter->entry_cur_id + 1);
        if (LIBYUC_CONTAINER_HASH_TABLE_COMPARER_Cmp(table, LIBYUC_CONTAINER_HASH_TABLE_ACCESSOR_GetKey(table, &link_entry->obj), key) == 0) {
            return &link_entry->obj;
        }
        iter->entry_prev_id = iter->entry_cur_id;
        iter->entry_cur_id = HashLinkSinglyListIteratorNext(&main_obj.head, iter->entry_cur_id);
    }
    return NULL;
}
void HashTableIteratorPut(HashTable* table, HashTableIterator* iter, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* obj) {
    HashTableEntry* entry = HashBucketVectorIndex(&table->bucket, iter->cur_index);
    HashLinkMainObj main_obj;
    main_obj.table = table;
    main_obj.head = entry->list_head;
    if (iter->entry_cur_id != HashLinkReferencer_InvalidId) {
        HashLinkEntry* link_entry = (HashLinkEntry*)HashLinkReferencer_Reference(&main_obj.head, iter->entry_cur_id);
        if (LIBYUC_CONTAINER_HASH_TABLE_COMPARER_Cmp(table, LIBYUC_CONTAINER_HASH_TABLE_ACCESSOR_GetKey(table, &link_entry->obj), LIBYUC_CONTAINER_HASH_TABLE_ACCESSOR_GetKey(table, obj)) == 0) {
            link_entry->obj = *obj;
            return;
        }
        HashLinkReferencer_Dereference(&main_obj.head, (HashLinkSinglyListEntry*)link_entry);
    }
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id link_entry_id = HashTableAllocLinkEntry(table);

    HashLinkVectorIndex(&table->link, link_entry_id + 1)->obj = *obj;
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
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id link_entry_id = iter->entry_cur_id;
    if (link_entry_id == HashLinkReferencer_InvalidId) return false;
    HashLinkSinglyListDeleteEntry(&main_obj.head, iter->entry_prev_id, link_entry_id);
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* old_element = &HashLinkVectorIndex(&table->link, link_entry_id + 1)->obj;
    HashBucketVectorSetCount(&table->bucket, HashBucketVectorGetCount(&table->bucket) - 1);
    entry->list_head = main_obj.head;
    HashTableFreeLinkEntry(table, link_entry_id);
    if (HashBucketVectorGetCapacity(&table->bucket) > LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE && HashGetCurrentLoadFator(table) <= 100 - table->load_fator) {
        HashRehash(table, HashBucketVectorGetCapacity(&table->bucket) / LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR);
    }
    return true;
}
LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* HashTableIteratorFirst(HashTable* table, HashTableIterator* iter) {
    iter->cur_index = 0;
    iter->entry_cur_id = HashLinkReferencer_InvalidId;
    return HashTableIteratorNext(table, iter);
}
LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* HashTableIteratorNext(HashTable* table, HashTableIterator* iter) {
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
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* element = &HashLinkVectorIndex(&table->link, iter->entry_cur_id + 1)->obj;
    return element;
}
   
LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* HashTableFind(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key* key) {
    HashTableIterator iter;
    return HashTableIteratorLocate(table, &iter, key);
}
void HashTablePut(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* obj) {
    const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key* key = LIBYUC_CONTAINER_HASH_TABLE_ACCESSOR_GetKey(table, obj);
    HashTableIterator iter;
    HashTableIteratorLocate(table, &iter, key);
    HashTableIteratorPut(table, &iter, obj);
}
bool HashTableDelete(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key* key) {
    HashTableIterator iter;
    HashTableIteratorLocate(table, &iter, key);
    return HashTableIteratorDelete(table, &iter);
}
   

