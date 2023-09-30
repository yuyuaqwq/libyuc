#include <libyuc/space_manager/object_pool.def>

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
    LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Id index = pool->first_entry.index;
    ObjectPoolBucketHashTable table;
    ObjectPoolBucketHashTableInit(&table, 0, 0);
    while (bucket_id != LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId) {
        LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* bucket = (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, bucket_id);
        ObjectPoolBucketEntry* bucket_entry = (ObjectPoolBucketEntry*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, &bucket, index);
        ObjectPoolBucketHashTablePut(&table, &bucket_id);
        bucket_id = bucket_entry->bucket_id;
        index = bucket_entry->index;
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
    }
    ObjectPoolBucketHashTableIterator iter;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id* bucket_id_ptr = ObjectPoolBucketHashTableIteratorFirst(&table, &iter);
    while (bucket_id_ptr) {
        LIBYUC_CONTAINER_OBJECT_POOL_ALLOCATOR_Release(pool, *bucket_id_ptr);
        bucket_id_ptr = ObjectPoolBucketHashTableIteratorNext(&table, &iter);
    }
    ObjectPoolBucketHashTableRelease(&table);
    pool->first_entry.bucket_id = LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId;
    pool->first_entry.index = 0;
}
/*
* 为便于使用，会直接返回被解引用后的内存地址，若不是内存对象池请注意返回地址是否可用
*/
LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* ObjectPoolAlloc(ObjectPool* pool, ObjectPoolBucketEntry* ret_entry) {
    if (pool->first_entry.bucket_id == LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId) {
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id bucket_id = (LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id)LIBYUC_CONTAINER_OBJECT_POOL_ALLOCATOR_CreateMultiple(pool, LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element, LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Const_StaticElementCount);
        LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* bucket = (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, bucket_id);
        for (ptrdiff_t i = 0; i < LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Const_StaticElementCount - 1; i++) {
            ObjectPoolBucketEntry* bucket_entry = (ObjectPoolBucketEntry*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, &bucket, i);
            bucket_entry->bucket_id = bucket_id;
            bucket_entry->index = i + 1;
        }
        ObjectPoolBucketEntry* bucket_entry = (ObjectPoolBucketEntry*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, &bucket, LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Const_StaticElementCount - 1);
        bucket_entry->bucket_id = pool->first_entry.bucket_id;
        bucket_entry->index = pool->first_entry.index;
           
        pool->first_entry.bucket_id = bucket_id;
        pool->first_entry.index = 0;
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
    }
    ret_entry->bucket_id = pool->first_entry.bucket_id;
    ret_entry->index = pool->first_entry.index;
    LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* bucket = (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, ret_entry->bucket_id);
    ObjectPoolBucketEntry* bucket_entry = (ObjectPoolBucketEntry*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, &bucket, ret_entry->index);
    pool->first_entry.bucket_id = bucket_entry->bucket_id;
    pool->first_entry.index = bucket_entry->index;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
    return (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)bucket_entry;
}
void ObjectPoolFree(ObjectPool* pool, ObjectPoolBucketEntry* free_entry) {
    LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* bucket = (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, free_entry->bucket_id);
    ObjectPoolBucketEntry* bucket_entry = (ObjectPoolBucketEntry*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, &bucket, free_entry->index);
    bucket_entry->bucket_id = pool->first_entry.bucket_id;
    bucket_entry->index = pool->first_entry.index;
    pool->first_entry.bucket_id = free_entry->bucket_id;
    pool->first_entry.index = free_entry->index;
    free_entry->bucket_id = LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId;
    free_entry->index = 0;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
}
    
#include <libyuc/space_manager/object_pool.undef>