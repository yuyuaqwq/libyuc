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

#include <libyuc/space_manager/object_pool.def>

/*
* 动态对象池
*/

#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME ObjectPoolBucket
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id
#include <libyuc/container/hash_table.h>

typedef struct ObjectPoolBucketEntry {
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id bucket_id; /* 下一个entry的bucket_id */
    LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Id index; /* 下一个entry的index */
} ObjectPoolBucketEntry;

typedef struct ObjectPool {
    ObjectPoolBucketEntry first_entry;
} ObjectPool;

void ObjectPoolBucketEntryInit(ObjectPoolBucketEntry* entry);
void ObjectPoolInit(ObjectPool* pool);
void ObjectPoolRelease(ObjectPool* pool);
LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* ObjectPoolAlloc(ObjectPool* pool, ObjectPoolBucketEntry* ret_entry);
void ObjectPoolFree(ObjectPool* pool, ObjectPoolBucketEntry* free_entry);


#include <libyuc/space_manager/object_pool.undef>


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_SPACE_MANAGER_OBJECT_POOL_H_