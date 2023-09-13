#ifdef __cplusplus
extern "C" {
#endif

#include <libyuc/container/vector.c>

/*
* 哈希表
*/
#define HashModIndex MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashModIndex)
#define HashGetIndex MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashGetIndex)
#define HashGetCurrentLoadFator MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashGetCurrentLoadFator)
#define HashEntryExchange MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashEntryExchange)
#define HashEntryMove MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashEntryMove)


static forceinline LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset HashModIndex(HashTable* table, LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset index) {
    return index & table->max_detection_count/* % HashBucketVectorGetCapacity(&table->bucket) */;
}
static forceinline LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset HashGetIndex(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_Type_Key* key) {
    return LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode(table, key) >> table->shift;
}
static forceinline LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset HashGetCurrentLoadFator(HashTable* table) {
    return HashBucketVectorGetCount(&table->bucket) * 100 / HashBucketVectorGetCapacity(&table->bucket);
}
static forceinline void HashEntryExchange(HashTable* table, HashTableEntry* entry_a, HashTableEntry* entry_b) {
    HashTableEntry temp;
    temp.dist = entry_a->dist;
    temp.obj = entry_a->obj;
    entry_a->dist = entry_b->dist;
    entry_a->obj = entry_b->obj;
    entry_b->dist = temp.dist;
    entry_b->obj = temp.obj;
}
static forceinline void HashEntryMove(HashTable* table, HashTableEntry* entry_dst, const HashTableEntry* entry_src) {
    entry_dst->dist = entry_src->dist;
    entry_dst->obj = entry_src->obj;
}

LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset HashTableGetCount(HashTable* table){ 
    return HashBucketVectorGetCount(&table->bucket); 
}
/* 重映射 */
static void HashRehash(HashTable* table, LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset new_capacity) {   
    HashTable temp_table;
    HashTableInit(&temp_table, new_capacity, table->load_fator);
    HashTableIterator iter;
    LIBYUC_CONTAINER_HASH_TABLE_Type_Element* obj = HashTableIteratorFirst(table, &iter);
    while (obj) {
        HashTablePut(&temp_table, obj);
        LIBYUC_CONTAINER_HASH_TABLE_Type_Key* key = LIBYUC_CONTAINER_HASH_TABLE_ACCESSOR_GetKey(table, obj);
        obj = HashTableIteratorNext(table, &iter);
    }
    HashTableRelease(table);
    MemoryCopy(table, &temp_table, sizeof(temp_table));
}
bool HashTableInit(HashTable* table, LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset capacity, LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset load_fator) {
    if (capacity < 2) {
        capacity = LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE;
    }
    if (!LIBYUC_ALGORITHM_TWO_IS_POWER_OF_2(capacity)) {
        capacity = LIBYUC_ALGORITHM_TWO_ALIGN_TO_POWER_OF_2(capacity);
    }
    HashBucketVectorInit(&table->bucket, capacity);
       
    for (int i = 0; i < HashBucketVectorGetCapacity(&table->bucket); i++) {
        HashBucketVectorIndex(&table->bucket, i)->dist = LIBYUC_CONTAINER_HASH_TABLE_DIST_INVALID_ID;
    }
    if (load_fator == 0) {
        load_fator = LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR;
    }
    table->load_fator = load_fator;
    table->max_detection_count = capacity - 1;
    table->shift = sizeof(LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode(table, ((LIBYUC_CONTAINER_HASH_TABLE_Type_Key*)NULL))) * 8;
    while (capacity /= 2) table->shift--;
    return true;
}
void HashTableRelease(HashTable* table) {
    HashBucketVectorRelease(&table->bucket);
}
forceinline LIBYUC_CONTAINER_HASH_TABLE_Type_Element* HashTableIteratorLocate(HashTable* table, HashTableIterator* iter, const LIBYUC_CONTAINER_HASH_TABLE_Type_Key* key) {
    iter->cur_index = HashGetIndex(table, key);
    iter->dist = LIBYUC_CONTAINER_HASH_TABLE_DIST_INVALID_ID;
       
    do {
        HashTableEntry* entry = HashBucketVectorIndex(&table->bucket, iter->cur_index);
        LIBYUC_CONTAINER_HASH_TABLE_Type_Key* entry_key = LIBYUC_CONTAINER_HASH_TABLE_ACCESSOR_GetKey(table, &entry->obj);
        if (LIBYUC_CONTAINER_HASH_TABLE_COMPARER_Equal(table, entry_key, key) && entry->dist != LIBYUC_CONTAINER_HASH_TABLE_DIST_INVALID_ID) {
            return &entry->obj;
        }
        if (iter->dist++ > entry->dist || entry->dist == LIBYUC_CONTAINER_HASH_TABLE_DIST_INVALID_ID) {
            return NULL;
        }
        iter->cur_index = HashModIndex(table, iter->cur_index + 1);
    } while (true);
    return NULL;
}
bool HashTableIteratorDelete(HashTable* table, HashTableIterator* iter) {
    HashTableEntry* empty_entry = HashBucketVectorIndex(&table->bucket, iter->cur_index);
    if (empty_entry == NULL) {
        return false;
    }
    iter->cur_index = HashModIndex(table, iter->cur_index + 1);
    HashTableEntry* next_entry = HashBucketVectorIndex(&table->bucket, iter->cur_index);
    while (next_entry->dist > 1) {
        --next_entry->dist;
        HashEntryMove(table, empty_entry, next_entry);
        empty_entry = next_entry;
        iter->cur_index = HashModIndex(table, iter->cur_index + 1);
        next_entry = HashBucketVectorIndex(&table->bucket, iter->cur_index);
    }
    empty_entry->dist = LIBYUC_CONTAINER_HASH_TABLE_DIST_INVALID_ID;
    HashBucketVectorSetCount(&table->bucket, HashBucketVectorGetCount(&table->bucket) - 1);
    if (HashBucketVectorGetCapacity(&table->bucket) > LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE && HashGetCurrentLoadFator(table) <= 100 - table->load_fator) {
        HashRehash(table, HashBucketVectorGetCapacity(&table->bucket) / LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR);
    }
    return true;
}
LIBYUC_CONTAINER_HASH_TABLE_Type_Element* HashTableIteratorFirst(HashTable* table, HashTableIterator* iter) {
    iter->cur_index = 0;
    return HashTableIteratorNext(table, iter);
}
LIBYUC_CONTAINER_HASH_TABLE_Type_Element* HashTableIteratorNext(HashTable* table, HashTableIterator* iter) {
    HashTableEntry* entry = NULL;
    do {
        if (iter->cur_index >= HashBucketVectorGetCapacity(&table->bucket)) {
            return NULL;
        }
        entry = HashBucketVectorIndex(&table->bucket, iter->cur_index++);
        if (entry->dist != LIBYUC_CONTAINER_HASH_TABLE_DIST_INVALID_ID) {
            break;
        }
    } while (true);
    return &entry->obj;
}
   
LIBYUC_CONTAINER_HASH_TABLE_Type_Element* HashTableFind(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_Type_Key* key) {
    HashTableIterator iter;
    return HashTableIteratorLocate(table, &iter, key);
}
void HashTablePut(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_Type_Element* obj) {
    LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset dist = 1;
    const LIBYUC_CONTAINER_HASH_TABLE_Type_Key* key = LIBYUC_CONTAINER_HASH_TABLE_ACCESSOR_GetKey(table, obj);
    LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset cur_index = HashGetIndex(table, key);
    HashTableEntry* entry;
    do {
        entry = HashBucketVectorIndex(&table->bucket, cur_index);
        if (dist == entry->dist && LIBYUC_CONTAINER_HASH_TABLE_COMPARER_Equal(table, LIBYUC_CONTAINER_HASH_TABLE_ACCESSOR_GetKey(table, &entry->obj), key)) {
            entry->obj = *obj;
            return;
        }
        if (dist > entry->dist) break;
        ++dist;
        cur_index = HashModIndex(table, cur_index + 1);
    } while(true);
    HashTableEntry temp;
    temp.dist = dist;
    temp.obj = *obj;
    while (entry->dist != 0) {
        HashEntryExchange(table, &temp, entry);
        ++temp.dist;
        cur_index = HashModIndex(table, cur_index + 1);
        entry = HashBucketVectorIndex(&table->bucket, cur_index);
    }
    HashEntryExchange(table, &temp, entry);
       
    HashBucketVectorSetCount(&table->bucket, HashBucketVectorGetCount(&table->bucket) + 1);
    if (HashGetCurrentLoadFator(table) >= table->load_fator) {
        /* 触发扩容 */
        HashRehash(table, HashBucketVectorGetCapacity(&table->bucket) * LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR);
    }
}
bool HashTableDelete(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_Type_Key* key) {
    HashTableIterator iter;
    HashTableIteratorLocate(table, &iter, key);
    return HashTableIteratorDelete(table, &iter);
}


#ifdef __cplusplus
}
#endif

#undef HashModIndex
#undef HashGetIndex
#undef HashGetCurrentLoadFatorr
#undef HashEntryExchange
#undef HashEntryMove