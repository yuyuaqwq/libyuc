/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_HASHTABLE_H_
#define CUTILS_CONTAINER_HASHTABLE_H_

#include <CUtils/object.h>
#include <CUtils/algorithm/hash_map.h>
#include <CUtils/container/array.h>
#include <CUtils/container/singly_list.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 链式哈希表
* 若objSize > sizeof(uintptr_t)，则存储指针
* 否则存储拷贝数据
*/

typedef enum _HashEntryType HashEntryType;

typedef struct _HashEntry {
    HashEntryType type;
    union {
        uintptr_t data;
        void* obj;
        SinglyListHead listHead;
    };
} HashEntry;

typedef struct _HashDataList {
    SinglyListEntry listEntry;
    union {
        uintptr_t data;
        void* obj;
    };
} HashDataList;

typedef struct _HashTable {
    Array bucket;
    // Array tempBucket;        // 保留，可优化为逐渐搬迁
    uint32_t loadFator;
    int keyFieldOffset;
    int keyFieldSize;
    int objSize;
    HashU32Func hashFunc;
    CmpFunc cmpFunc;
} HashTable;

typedef struct _HashTableIterator {
    HashTable* table;
    int curIndex;
    HashDataList* curListEntry;
#ifdef HASHTABLE_DATA_STATISTICS
    int freeCount;
    int objCount;
    int listEntryCount;
    int listHeadCount;
    int maxListCount;
    int curListCount;
#endif // HASHTABLE_DATA_STATISTICS
} HashTableIterator;

// #define HASHTABLE_DATA_STATISTICS

#define HASHTABLE_DEFAULT_BUCKETS_SIZE 16
#define HASHTABLE_DEFAULT_LOAD_FACTOR 75//%
#define HASHTABLE_DEFAULT_EXPANSION_FACTOR 2

void HashTableInit(HashTable* table, size_t capacity, uint32_t loadFator, int objSize, int keyFieldOffset, int keySize, HashU32Func hashFunc, CmpFunc cmpFunc);
void HashEntryInit(HashEntry* entry);
void HashTableRelease(HashTable* table, bool deleteObj);
size_t HashTableGetCount(HashTable* table);
size_t HashTableGetCapacity(HashTable* table);
void* HashTableFind(HashTable* table, void* key);
bool HashTableInsert(HashTable* table, void* obj);
void* HashTableDelete(HashTable* table, void* key);


void* HashTableNext(HashTableIterator* iter);
void* HashTableFirst(HashTable* table, HashTableIterator* iter);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_HASHTABLE_H_