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

typedef struct ObjectPoolSlot {
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id next_block_id; /* 下一个slot的block_id */
    LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Id next_slot_index; /* 下一个slot的index */
} ObjectPoolSlot;

typedef struct ObjectPool {
    ObjectPoolSlot first_slot;
} ObjectPool;

void ObjectPoolSlotInit(ObjectPoolSlot* slot);
void ObjectPoolInit(ObjectPool* pool);
void ObjectPoolRelease(ObjectPool* pool);
LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* ObjectPoolAlloc(ObjectPool* pool, ObjectPoolSlot* ret_slot);
void ObjectPoolFree(ObjectPool* pool, ObjectPoolSlot* free_slot);


#include <libyuc/space_manager/object_pool.undef>



#endif // LIBYUC_SPACE_MANAGER_OBJECT_POOL_H_