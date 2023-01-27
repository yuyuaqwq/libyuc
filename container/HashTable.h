/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_HASHTABLE_H_
#define CUTILS_CONTAINER_HASHTABLE_H_

#include "CUtils/object.h"
#include "CUtils/algorithm/HashMap.h"
#include "CUtils/container/array.h"
#include "CUtils/container/SinglyList.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    kFree,
    kObj,
    kList,
} HashEntryType;

typedef struct _HashEntry {
    HashEntryType type;
    union {
        void* obj;
        SinglyListHead listHead;
    };
    
} HashEntry;

typedef struct _HashDataList {
    SinglyListEntry listEntry;
    void* obj;
} HashDataList;

typedef struct _HashTable {
    Array bucket;
    // Array tempBucket;        // 保留，未来可能修改为逐渐搬迁
    int keyFieldOffset;
    int keyFieldSize;
    int loadFator;
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
#define HASHTABLE_DEFAULT_LOAD_FATOR 75//%

void HashTableInit(HashTable* table, int capacity, int keyFieldOffset, int keySize, HashU32Func hashFunc, CmpFunc cmpFunc);
void HashEntryInit(HashEntry* entry);
void HashTableRelease(HashTable* table, bool deleteObj);
void* HashTableFind(HashTable* table, void* key);
bool HashTableInsert(HashTable* table, void* obj);
void* HashTableDelete(HashTable* table, void* key);

void* HashTableNext(HashTableIterator* iter);
void* HashTableFirst(HashTable* table, HashTableIterator* iter);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_HASHTABLE_H_