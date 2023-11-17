/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_SPACE_MANAGER_OBJECT_POOL_H_
#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_H_

#include <libyuc/basic.h>

#include <libyuc/space_manager/object_pool.def>

#define LIBYUC_CONTAINER_VECTOR_CLASS_NAME MAKE_NAME(LIBYUC_SPACE_MANAGER_OBJECT_POOL_CLASS_NAME, ObjectPoolBlock)
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element union ObjectPoolSlot*      // &block[0]
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
#include <libyuc/container/vector.h>


/*
* 动态对象池
*/

#if defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_INDEX)
typedef LIBYUC_SPACE_MANAGER_OBJECT_POOL_Pos_Type_Id ObjectPoolSlotPos;
typedef LIBYUC_SPACE_MANAGER_OBJECT_POOL_Pos_Type_Id ObjectPoolBlockId;
typedef LIBYUC_SPACE_MANAGER_OBJECT_POOL_Pos_Type_Id ObjectPoolSlotId;
#elif defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_PTR)
typedef union ObjectPoolSlot* ObjectPoolSlotPos;
#endif
typedef LIBYUC_SPACE_MANAGER_OBJECT_POOL_Pos_Type_Id ObjectPoolSlotSize;

typedef struct ObjectPool {
    ObjectPoolBlockVector block_table;

    ObjectPoolSlotPos free_slot;

    ObjectPoolSlotPos begin_slot;
    ObjectPoolSlotPos end_slot;
} ObjectPool;

void ObjectPoolSlotInit(ObjectPoolSlotPos* slot);
void ObjectPoolInit(ObjectPool* pool);
void ObjectPoolRelease(ObjectPool* pool);

#if defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_INDEX)
ObjectPoolSlotPos ObjectPoolAlloc(ObjectPool* pool);
void ObjectPoolFree(ObjectPool* pool, ObjectPoolSlotPos free_pos);
#elif defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_PTR)
LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Element* ObjectPoolAlloc(ObjectPool* pool);
void ObjectPoolFree(ObjectPool* pool, LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Element* free_ele);
#endif



#include <libyuc/space_manager/object_pool.undef>


#endif // LIBYUC_SPACE_MANAGER_OBJECT_POOL_H_