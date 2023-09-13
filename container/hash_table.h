/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_HASH_TABLE_H_
#define LIBYUC_CONTAINER_HASH_TABLE_H_

#include <libyuc/basic.h>
#include <libyuc/algorithm/two.h>
#include <libyuc/algorithm/hash_code.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifndef LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME
#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_Type_Element
#define LIBYUC_CONTAINER_HASH_TABLE_Type_Element int
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_Type_Key
#define LIBYUC_CONTAINER_HASH_TABLE_Type_Key LIBYUC_CONTAINER_HASH_TABLE_Type_Element
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_ACCESSOR_GetKey
#define LIBYUC_CONTAINER_HASH_TABLE_ACCESSOR_GetKey(main_obj, obj) (obj)
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Const_InvalidId
#define LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Const_InvalidId (-1)
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Id
#define LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Id size_t
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset
#define LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Id
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode
#define LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode(main_obj, obj) HashCode_hashint(*obj)
#endif

#ifndef LIBYUC_CONTAINER_HASH_TABLE_COMPARER_Equal
#define LIBYUC_CONTAINER_HASH_TABLE_COMPARER_Equal(main_obj, obj1, obj2) (*(obj1) == *(obj2))
#endif

/*
* 基于robin hood hashing的哈希表实现
* 参考https://github.com/martinus/unordered_dense
*/
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_BUCKETS_SIZE 16
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_LOAD_FACTOR 75    //%
#define LIBYUC_CONTAINER_HASH_TABLE_DEFAULT_EXPANSION_FACTOR 2
#define LIBYUC_CONTAINER_HASH_TABLE_DIST_INVALID_ID 0

#define HashTable MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashTable)
#define HashTableEntry MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashTableEntry)
#define HashTableIterator MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashTableIterator)


typedef struct HashTableIterator {
    LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Id dist;
    LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Id cur_index;
} HashTableIterator;

typedef struct HashTableEntry {
    LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Id dist; /* 与第一次哈希相差的距离，从1开始算起(0表示无效) */
    LIBYUC_CONTAINER_HASH_TABLE_Type_Element obj;
} HashTableEntry;


#define LIBYUC_CONTAINER_VECTOR_CLASS_NAME MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashBucket)
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
#define LIBYUC_CONTAINER_VECTOR_Type_Element HashTableEntry
#define LIBYUC_CONTAINER_VECTOR_REFERENCER_Type_Id LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Id
#include <libyuc/container/vector.h>




typedef struct HashTable {
    LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset max_detection_count;
    LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset load_fator;
    LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset shift;
    Vector bucket;
} HashTable;





#define HashTableInit MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashTableInit)
#define HashTableRelease MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashTableRelease)
#define HashTableGetCount MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashTableGetCount)
#define HashTableFind MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashTableFind)
#define HashTablePut MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashTablePut)
#define HashTableDelete MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashTableDelete)
#define HashTableIteratorFirst MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashTableIteratorFirst)
#define HashTableIteratorNext MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashTableIteratorNext)
#define HashTableIteratorLocate MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashTableIteratorLocate)
#define HashTableIteratorDelete MAKE_NAME(LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME, HashTableIteratorDelete)

   
bool HashTableInit(HashTable* table, LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset capacity, LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset load_fator);
void HashTableRelease(HashTable* table);
LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset HashTableGetCount(HashTable* table);
LIBYUC_CONTAINER_HASH_TABLE_Type_Element* HashTableFind(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_Type_Key* key);
void HashTablePut(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_Type_Element* obj);
bool HashTableDelete(HashTable* table, const LIBYUC_CONTAINER_HASH_TABLE_Type_Key* key);

LIBYUC_CONTAINER_HASH_TABLE_Type_Element* HashTableIteratorFirst(struct HashTable* table, HashTableIterator* iter);
LIBYUC_CONTAINER_HASH_TABLE_Type_Element* HashTableIteratorNext(struct HashTable* table, HashTableIterator* iter);
LIBYUC_CONTAINER_HASH_TABLE_Type_Element* HashTableIteratorLocate(struct HashTable* table, HashTableIterator* iter, const LIBYUC_CONTAINER_HASH_TABLE_Type_Key* key);
bool HashTableIteratorDelete(struct HashTable* table, HashTableIterator* iter);


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_HASH_TABLE_H_

#ifndef LIBYUC_TEMPLATE_C_INCLUDE
#undef HashTable
#undef HashTableEntry
#undef HashTableIterator

#undef HashTableInit
#undef HashTableRelease
#undef HashTableGetCount
#undef HashTableFind
#undef HashTablePut
#undef HashTableDelete
#undef HashTableIteratorFirst
#undef HashTableIteratorNext
#undef HashTableIteratorLocate
#undef HashTableIteratorDelete


#undef LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME
#undef LIBYUC_CONTAINER_HASH_TABLE_Type_Element
#undef LIBYUC_CONTAINER_HASH_TABLE_Type_Key
#undef LIBYUC_CONTAINER_HASH_TABLE_ACCESSOR_GetKey
#undef LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Const_InvalidId
#undef LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Id
#undef LIBYUC_CONTAINER_HASH_TABLE_REFERENCER_Type_Offset
#undef LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Release
#undef LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode
#undef LIBYUC_CONTAINER_HASH_TABLE_COMPARER_Equal
#endif