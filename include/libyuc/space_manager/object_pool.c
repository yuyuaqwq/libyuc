#include <libyuc/space_manager/object_pool.def>

/*
* 优化思路：
* 为pool分配一张block表，每个元素存放指向实际block的指针(可以动态扩容)，block_id是表中的索引
*/

#define LIBYUC_CONTAINER_VECTOR_CLASS_NAME ObjectPoolBlock
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Element*
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
#include <libyuc/container/vector.c>

void ObjectPoolSlotInit(ObjectPoolSlotPos* entry) {
    entry->block_id = LIBYUC_SPACE_MANAGER_OBJECT_POOL_BLOCK_INDEXER_Const_InvalidId;
    entry->slot_id = 0;
}
void ObjectPoolInit(ObjectPool* pool) {
    ObjectPoolSlotInit(&pool->first);
}
/*
* 释放整个对象池
*/
void ObjectPoolRelease(ObjectPool* pool) {
    for (size_t i = 0; i < ObjectPoolBlockVectorGetCount(&pool->block_table); i++) {
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_BLOCK_ALLOCATOR_Release(pool, *ObjectPoolBlockVectorIndex(&pool->block_table, i));
    }
    ObjectPoolBlockVectorRelease(&pool->block_table);
}

LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Element* ObjectPoolGetPtr(ObjectPool* pool, ObjectPoolSlotPos* pos) {
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Element* block = *ObjectPoolBlockVectorIndex(&pool->block_table, pos->block_id);
    return LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_GetPtr(pool, block, pos->slot_id);
}

/*
* 分配，返回pos
*/
ObjectPoolSlotPos ObjectPoolAlloc(ObjectPool* pool) {
    ObjectPoolSlotPos ret_pos;
    if (pool->first.block_id == LIBYUC_SPACE_MANAGER_OBJECT_POOL_BLOCK_INDEXER_Const_InvalidId) {
        // 没有空闲的块
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Element* block = LIBYUC_SPACE_MANAGER_OBJECT_POOL_BLOCK_ALLOCATOR_CreateMultiple(pool, LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Element, LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Const_StaticElementCount);
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_BLOCK_INDEXER_Type_Id next_block_id = ObjectPoolBlockVectorGetCount(&pool->block_table);
        ObjectPoolBlockVectorPushTail(&pool->block_table, (const LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Element**)&block);
        for (ptrdiff_t i = 0; i < LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Const_StaticElementCount - 1; i++) {
            ObjectPoolSlotPos* next_slot_entry = (ObjectPoolSlotPos*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_GetPtr(pool, block, i);
            next_slot_entry->block_id = next_block_id;
            next_slot_entry->slot_id = i + 1;
        }
        ObjectPoolSlotPos* next_slot = (ObjectPoolSlotPos*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_GetPtr(pool, block, LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Const_StaticElementCount - 1);
        next_slot->block_id = pool->first.block_id;
        next_slot->slot_id = pool->first.slot_id;

        pool->first.block_id = next_block_id;
        pool->first.slot_id = 0;
    }
    // 取第一个空闲的slot
    ret_pos.block_id = pool->first.block_id;
    ret_pos.slot_id = pool->first.slot_id;
    ObjectPoolSlotPos* next_slot = (ObjectPoolSlotPos*)ObjectPoolGetPtr(pool, &ret_pos);
    pool->first.block_id = next_slot->block_id;
    pool->first.slot_id = next_slot->slot_id;
    return ret_pos;
}

void ObjectPoolFree(ObjectPool* pool, ObjectPoolSlotPos* free_pos) {
    ObjectPoolSlotPos* slot = (ObjectPoolSlotPos*)ObjectPoolGetPtr(pool, free_pos);
    slot->block_id = pool->first.block_id;
    slot->slot_id = pool->first.slot_id;
    pool->first.block_id = free_pos->block_id;
    pool->first.slot_id = free_pos->slot_id;
    free_pos->block_id = LIBYUC_SPACE_MANAGER_OBJECT_POOL_BLOCK_INDEXER_Const_InvalidId;
    free_pos->slot_id = 0;
}



#include <libyuc/space_manager/object_pool.undef>