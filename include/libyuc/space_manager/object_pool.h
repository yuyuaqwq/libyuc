/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_SPACE_MANAGER_OBJECT_POOL_H_
#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_H_

#include <libyuc/basic.h>


#include <libyuc/space_manager/object_pool.def>

/*
* 动态对象池
*/

#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME ObjectPoolBlock
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element uintptr_t      // LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id
#include <libyuc/container/hash_table.h>

typedef struct ObjectPoolSlotPos {
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id block_id; /* slot的block_id */
    LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Id slot_id; /* slot的index */
} ObjectPoolSlotPos;

typedef struct ObjectPool {
    ObjectPoolSlotPos first;
} ObjectPool;

void ObjectPoolSlotInit(ObjectPoolSlotPos* slot);
void ObjectPoolInit(ObjectPool* pool);
void ObjectPoolRelease(ObjectPool* pool);
LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* ObjectPoolAlloc(ObjectPool* pool, ObjectPoolSlotPos* ret_pos);
void ObjectPoolFree(ObjectPool* pool, ObjectPoolSlotPos* free_pos);


#include <libyuc/space_manager/object_pool.undef>



#endif // LIBYUC_SPACE_MANAGER_OBJECT_POOL_H_