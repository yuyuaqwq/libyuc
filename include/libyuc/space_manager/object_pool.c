#include <libyuc/space_manager/object_pool.def>


#define LIBYUC_CONTAINER_VECTOR_CLASS_NAME MAKE_NAME(LIBYUC_SPACE_MANAGER_OBJECT_POOL_CLASS_NAME, ObjectPoolBlock)
#define LIBYUC_CONTAINER_VECTOR_INDEXER_Type_Element union ObjectPoolSlot*
#define LIBYUC_CONTAINER_VECTOR_MODE_DYNAMIC
#include <libyuc/container/vector.c>

#define LIBYUC_SPACE_MANAGER_OBJECT_POOL_Const_InvalidId (-1)


typedef union ObjectPoolSlot {
    ObjectPoolSlotPos next;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Element element;
} ObjectPoolSlot;

#if defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_INDEX)
#define kSlotPosInvalid = -1
#elif defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_PTR)
#define kSlotPosInvalid NULL
#endif
#define kBlockSlotCount (4096 / sizeof(ObjectPoolSlot))

//#define kBlockShift 10;    // 此处应当通过计算得出
//#define kSlotMark (kBlockSlotCount - 1);


void ObjectPoolInit(ObjectPool* pool) {
    pool->free_slot = kSlotPosInvalid;
    ObjectPoolBlockVectorInit(&pool->block_table, 8);
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

#if defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_INDEX)
static void ObjectPoolSplitId(ObjectPoolSlotPos slot_pos, ObjectPoolBlockId* block_id, ObjectPoolSlotId* slot_id) {
    // 交给编译器优化
    *block_id = slot_pos / kBlockSlotCount;
    *slot_id = slot_pos % kBlockSlotCount;
    //*block_id = slot_pos >> kBlockShift;
    //*slot_id = slot_pos & kSlotMark;
}

ObjectPoolSlot* ObjectPoolGetPtr(ObjectPool* pool, ObjectPoolSlotPos pos) {
    ObjectPoolBlockId block_id;
    ObjectPoolSlotId slot_id;
    ObjectPoolSplitId(pos, &block_id, &slot_id);
    ObjectPoolSlot* block = *ObjectPoolBlockVectorIndex(&pool->block_table, block_id);
    return LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_GetPtr(pool, block, slot_id);
}
#endif

/*
* 分配，返回pos
*/
#if defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_INDEX)
ObjectPoolSlotPos ObjectPoolAlloc(ObjectPool* pool) {
#elif defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_PTR)
LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Element* ObjectPoolAlloc(ObjectPool* pool) {
#endif
    ObjectPoolSlotPos ret_pos;
    if (pool->free_slot != kSlotPosInvalid) {
        ret_pos = pool->free_slot;
#if defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_INDEX)
        ObjectPoolBlockId block_id;
        ObjectPoolSlotId slot_id;
        ObjectPoolSplitId(ret_pos, &block_id, &slot_id);
        ObjectPoolSlot* block = *ObjectPoolBlockVectorIndex(&pool->block_table, block_id);
        pool->free_slot = block[slot_id].next;
        return ret_pos;
#elif defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_PTR)
        pool->free_slot = pool->free_slot->next;
        return &ret_pos->element;
#endif
    }
    else {
        if (pool->begin_slot >= pool->end_slot) {
            // 没有空闲的块
            ObjectPoolSlot* block = LIBYUC_SPACE_MANAGER_OBJECT_POOL_BLOCK_ALLOCATOR_CreateMultiple(pool, ObjectPoolSlot, kBlockSlotCount);
#if defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_INDEX)
            ObjectPoolSlotPos slot_pos = ObjectPoolBlockVectorGetCount(&pool->block_table) * kBlockSlotCount;
#elif defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_PTR)
            ObjectPoolSlotPos slot_pos = block;
#endif
            ObjectPoolBlockVectorPushTail(&pool->block_table, (const ObjectPoolSlot**)&block);

            pool->begin_slot = slot_pos;
            pool->end_slot = slot_pos + kBlockSlotCount;
        }
#if defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_INDEX)
        return pool->begin_slot++;
#elif defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_PTR)
        return &(pool->begin_slot++)->element;
#endif
    }
}

#if defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_INDEX)
void ObjectPoolFree(ObjectPool* pool, ObjectPoolSlotPos free_ele) {
#elif defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_PTR)
void ObjectPoolFree(ObjectPool * pool, LIBYUC_SPACE_MANAGER_OBJECT_POOL_SLOT_INDEXER_Type_Element* free_ele) {
#endif
    ObjectPoolSlotPos free_pos = (ObjectPoolSlotPos)free_ele;
    if (free_pos != kSlotPosInvalid) {
#if defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_INDEX)
        ObjectPoolBlockId block_id;
        ObjectPoolSlotId slot_id;
        ObjectPoolSplitId(free_pos, &block_id, &slot_id);

        ObjectPoolSlot* block = *ObjectPoolBlockVectorIndex(&pool->block_table, block_id);
        block[slot_id].next = pool->free_slot;
#elif defined(LIBYUC_SPACE_MANAGER_OBJECT_POOL_MODE_PTR)
        free_pos->next = pool->free_slot;
#endif
        pool->free_slot = free_pos;
    }
}

#undef kSlotPosInvalid
#undef kBlockSlotCount
#undef kBlockShift
#undef kSlotMark 

#include <libyuc/space_manager/object_pool.undef>