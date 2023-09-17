/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_SPACE_MANAGER_OBJECT_POOL_H_
#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_H_

#include <libyuc/basic.h>
//

#ifdef __cplusplus
extern "C" {
#endif

/*
* 动态对象池
*/
#ifndef LIBYUC_SPACE_MANAGER_OBJECT_POOL_CLASS_NAME
#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_CLASS_NAME
#endif

#ifndef LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id
#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id struct ListEntry*
#endif

#ifndef LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Offset
#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Offset size_t
#endif

#ifndef LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId
#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId NULL
#endif

#ifndef LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference
#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(main_obj, reference) (reference) 
#endif

#ifndef LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference
#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(main_obj, reference)
#endif

#ifndef LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Create
#define LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Create(main_obj, obj_type) MemoryAlloc(sizeof(obj_type))
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_ALLOCATOR_CreateMultiple
#define LIBYUC_CONTAINER_VECTOR_ALLOCATOR_CreateMultiple(main_obj, obj_type, count) ((obj_type*)MemoryAlloc(sizeof(obj_type) * count))
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Release
#define LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Release(main_obj, obj) MemoryFree(obj)
#endif

#ifndef LIBYUC_CONTAINER_VECTOR_CALLBACK_Expand
#define LIBYUC_CONTAINER_VECTOR_CALLBACK_Expand
#endif


#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_Type_DynamicArray
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Type_DynamicArray LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element*
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element int
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_Const_StaticElementCount
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Const_StaticElementCount 512
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_Const_InvalidId
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Const_InvalidId (-1)
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_Const_InvalidDynamicArray
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Const_InvalidDynamicArray (NULL)
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id size_t
#endif
#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Offset LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id
#endif

#ifndef LIBYUC_CONTAINER_VECTOR_INDEXER_GetPtr
#define LIBYUC_CONTAINER_VECTOR_INDEXER_GetPtr(main_obj, arr, index) (&(arr)[(index)])
#endif

#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME ObjectPoolBucket
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id
#include <libyuc/container/hash_table.h>

typedef struct ObjectPoolBucketEntry {
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id bucket_id; /* 下一个entry的bucket_id */
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id index; /* 下一个entry的index */
} ObjectPoolBucketEntry;

typedef struct ObjectPool {
    ObjectPoolBucketEntry first_entry;
} ObjectPool;

void ObjectPoolBucketEntryInit(ObjectPoolBucketEntry* entry);
void ObjectPoolInit(ObjectPool* pool);
void ObjectPoolRelease(ObjectPool* pool);
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* ObjectPoolAlloc(ObjectPool* pool, ObjectPoolBucketEntry* ret_entry);
void ObjectPoolFree(ObjectPool* pool, ObjectPoolBucketEntry* free_entry);


void ObjectPoolBucketEntryInit(ObjectPoolBucketEntry* entry) {
    entry->bucket_id = LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId;
    entry->index = 0;
}
void ObjectPoolInit(ObjectPool* pool) {
    ObjectPoolBucketEntryInit(&pool->first_entry);
}
/*
* 将所有分配的对象释放后，再调用该函数释放整个对象池，否则可能出现内存泄露
*/
void ObjectPoolRelease(ObjectPool* pool) {
    if (pool->first_entry.bucket_id == LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId) return;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id bucket_id = pool->first_entry.bucket_id;
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Id index = pool->first_entry.index;
    ObjectPoolBucketHashTable table;
    ObjectPoolBucketHashTableInit(&table, 0, 0);
    while (bucket_id != LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId) {
        LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* bucket = (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, bucket_id);
        ObjectPoolBucketEntry* bucket_entry = (ObjectPoolBucketEntry*)LIBYUC_CONTAINER_VECTOR_INDEXER_GetPtr(pool, &bucket, index);
        ObjectPoolBucketHashTablePut(&table, &bucket_id);
        bucket_id = bucket_entry->bucket_id;
        index = bucket_entry->index;
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
    }
    ObjectPoolBucketHashTableIterator iter;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id* bucket_id_ptr = ObjectPoolBucketHashTableIteratorFirst(&table, &iter);
    while (bucket_id_ptr) {
        LIBYUC_CONTAINER_VECTOR_ALLOCATOR_Release(pool, *bucket_id_ptr);
        bucket_id_ptr = ObjectPoolBucketHashTableIteratorNext(&table, &iter);
    }
    ObjectPoolBucketHashTableRelease(&table);
    pool->first_entry.bucket_id = LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId;
    pool->first_entry.index = 0;
}
/*
* 为便于使用，会直接返回被解引用后的内存地址，若不是内存对象池请注意返回地址是否可用
*/
LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* ObjectPoolAlloc(ObjectPool* pool, ObjectPoolBucketEntry* ret_entry) {
    if (pool->first_entry.bucket_id == LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId) {
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id bucket_id = (LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id)LIBYUC_CONTAINER_VECTOR_ALLOCATOR_CreateMultiple(pool, LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element, LIBYUC_CONTAINER_VECTOR_INDEXER_Const_StaticElementCount);
        LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* bucket = (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, bucket_id);
        for (ptrdiff_t i = 0; i < LIBYUC_CONTAINER_VECTOR_INDEXER_Const_StaticElementCount - 1; i++) {
            ObjectPoolBucketEntry* bucket_entry = (ObjectPoolBucketEntry*)LIBYUC_CONTAINER_VECTOR_INDEXER_GetPtr(pool, &bucket, i);
            bucket_entry->bucket_id = bucket_id;
            bucket_entry->index = i + 1;
        }
        ObjectPoolBucketEntry* bucket_entry = (ObjectPoolBucketEntry*)LIBYUC_CONTAINER_VECTOR_INDEXER_GetPtr(pool, &bucket, LIBYUC_CONTAINER_VECTOR_INDEXER_Const_StaticElementCount - 1);
        bucket_entry->bucket_id = pool->first_entry.bucket_id;
        bucket_entry->index = pool->first_entry.index;
           
        pool->first_entry.bucket_id = bucket_id;
        pool->first_entry.index = 0;
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
    }
    ret_entry->bucket_id = pool->first_entry.bucket_id;
    ret_entry->index = pool->first_entry.index;
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* bucket = (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, ret_entry->bucket_id);
    ObjectPoolBucketEntry* bucket_entry = (ObjectPoolBucketEntry*)LIBYUC_CONTAINER_VECTOR_INDEXER_GetPtr(pool, &bucket, ret_entry->index);
    pool->first_entry.bucket_id = bucket_entry->bucket_id;
    pool->first_entry.index = bucket_entry->index;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
    return (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element*)bucket_entry;
}
void ObjectPoolFree(ObjectPool* pool, ObjectPoolBucketEntry* free_entry) {
    LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element* bucket = (LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, free_entry->bucket_id);
    ObjectPoolBucketEntry* bucket_entry = (ObjectPoolBucketEntry*)LIBYUC_CONTAINER_VECTOR_INDEXER_GetPtr(pool, &bucket, free_entry->index);
    bucket_entry->bucket_id = pool->first_entry.bucket_id;
    bucket_entry->index = pool->first_entry.index;
    pool->first_entry.bucket_id = free_entry->bucket_id;
    pool->first_entry.index = free_entry->index;
    free_entry->bucket_id = LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId;
    free_entry->index = 0;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
}
    

//LIBYUC_SPACE_MANAGER_OBJECT_POOL_DECLARATION(Int, int64_t, struct _IntObjectPoolBucketEntry*, int16_t)
//LIBYUC_SPACE_MANAGER_OBJECT_POOL_DEFINE(Int, int64_t, struct _IntObjectPoolBucketEntry*, int16_t, LIBYUC_SPACE_MANAGER_OBJECT_POOL_DEFAULT_ACCESSOR, LIBYUC_BASIC_ALLOCATOR_DEFALUT, LIBYUC_BASIC_REFERENCER_DEFALUT)

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_SPACE_MANAGER_OBJECT_POOL_H_