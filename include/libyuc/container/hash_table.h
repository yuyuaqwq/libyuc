/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>
#include <libyuc/algorithm/two.h>
#include <libyuc/algorithm/hash_code.h>

/*
* 基于robin hood hashing的哈希表实现
* 参考https://github.com/martinus/unordered_dense
*/

// #define LIBYUC_CONTAINER_HASH_TABLE_MODE_LRU

#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE 16
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR 75    //%
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR 2
#define LIBYUC_CONTAINER_HASH_TABLE_DIST_INVALID_ID 0


#include <libyuc/container/hash_table.def>


typedef struct HashTableIterator {
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id dist;
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id cur_index;
} HashTableIterator;

#ifdef LIBYUC_CONTAINER_HASH_TABLE_MODE_LRU
#include <libyuc/container/hash_table_HashLruDoublyStaticList.cfg>
#include <libyuc/container/doubly_static_list.h>
#endif

typedef struct HashTableEntry {
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Id dist; /* 与第一次哈希相差的距离，从1开始算起(0表示无效) */
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element obj;
#ifdef LIBYUC_CONTAINER_HASH_TABLE_MODE_LRU
    HashLruDoublyStaticListEntry lru_entry;
#endif
} HashTableEntry;


#include <libyuc/container/hash_table_HashBucketVector.cfg>
#include <libyuc/container/vector.h>


typedef struct HashTable {
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Offset max_detection_count;
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Offset load_fator;
    LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Offset shift;
    HashBucketVector bucket;
} HashTable;


bool HashTableInit(HashTable* table, LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Offset capacity, LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Offset load_fator);
void HashTableRelease(HashTable* table);
LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Offset HashTableGetCount(HashTable* table);
LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* HashTableFind(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key* key);
void HashTablePut(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* obj);
bool HashTableDelete(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key* key);

LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* HashTableIteratorFirst(struct HashTable* table, HashTableIterator* iter);
LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* HashTableIteratorNext(struct HashTable* table, HashTableIterator* iter);
LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element* HashTableIteratorLocate(struct HashTable* table, HashTableIterator* iter, const LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Key* key);
bool HashTableIteratorDelete(struct HashTable* table, HashTableIterator* iter);


#include <libyuc/container/hash_table.undef>


