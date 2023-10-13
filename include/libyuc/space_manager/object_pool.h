/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_SPACE_MANAGER_OBJECT_POOL_H_
#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_H_

#include <libyuc/basic.h>


#include <libyuc/space_manager/object_pool.def>

#define LIBYUC_CONTAINER_VECTOR_CLASS_NAME ObjectPoolBlock
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Element*      // &block[0]
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
#include <libyuc/container/vector.h>

/*
* 动态对象池
*/

typedef struct ObjectPoolSlotPos {
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_BLOCK_INDEXER_Type_Id block_id; /* slot的block_id */
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Id slot_id; /* slot的index */
} ObjectPoolSlotPos;

typedef struct ObjectPool {
    ObjectPoolSlotPos first;
    ObjectPoolBlockVector block_table;
} ObjectPool;

void ObjectPoolSlotInit(ObjectPoolSlotPos* slot);
void ObjectPoolInit(ObjectPool* pool);
void ObjectPoolRelease(ObjectPool* pool);
ObjectPoolSlotPos ObjectPoolAlloc(ObjectPool* pool);
void ObjectPoolFree(ObjectPool* pool, ObjectPoolSlotPos* free_pos);


#include <libyuc/space_manager/object_pool.undef>



#endif // LIBYUC_SPACE_MANAGER_OBJECT_POOL_H_