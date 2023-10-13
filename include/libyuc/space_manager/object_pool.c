#include <libyuc/space_manager/object_pool.def>

/*
* 优化思路：
* 为pool分配一张block表，每个元素存放指向实际block的指针(可以动态扩容)，block_id是表中的索引
*/

#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME ObjectPoolBlock
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element uintptr_t
#define LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode(main_obj, obj) (_wymix(*(obj), UINT64_C(0x9E3779B97F4A7C15)))
#include <libyuc/container/hash_table.c>

void ObjectPoolSlotInit(ObjectPoolSlotPos* entry) {
    entry->block_id = LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId;
    entry->slot_id = 0;
}
void ObjectPoolInit(ObjectPool* pool) {
    ObjectPoolSlotInit(&pool->first);
}
/*
* 将所有分配的对象释放后，再调用该函数释放整个对象池，否则可能出现内存泄露
* 通过block表则不需要释放整个对象池
*/
void ObjectPoolRelease(ObjectPool* pool) {
    if (pool->first.block_id == LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId) return;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id next_block_id = pool->first.block_id;
    LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Id next_slot_id = pool->first.slot_id;
    ObjectPoolBlockHashTable table;
    ObjectPoolBlockHashTableInit(&table, 0, 0);
    while (next_block_id != LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId) {
        LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* bucket = (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, next_block_id);
        ObjectPoolSlotPos* next_slot = (ObjectPoolSlotPos*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, &bucket, next_slot_id);
        ObjectPoolBlockHashTablePut(&table, (const uintptr_t*)&next_block_id);
        next_block_id = next_slot->block_id;
        next_slot_id = next_slot->slot_id;
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
    }
    ObjectPoolBlockHashTableIterator iter;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id* next_block_id_ptr = (LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id*)ObjectPoolBlockHashTableIteratorFirst(&table, &iter);
    while (next_block_id_ptr) {
        LIBYUC_CONTAINER_OBJECT_POOL_ALLOCATOR_Release(pool, *next_block_id_ptr);
        next_block_id_ptr = (LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id*)ObjectPoolBlockHashTableIteratorNext(&table, &iter);
    }
    ObjectPoolBlockHashTableRelease(&table);
    pool->first.block_id = LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId;
    pool->first.slot_id = 0;
}
/*
* 为便于使用，会直接返回被解引用后的内存地址，若不是内存对象池请注意返回地址是否可用
*/
LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* ObjectPoolAlloc(ObjectPool* pool, ObjectPoolSlotPos* ret_pos) {
    if (pool->first.block_id == LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId) {
        // 没有空闲的块
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id next_block_id = (LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id)LIBYUC_CONTAINER_OBJECT_POOL_ALLOCATOR_CreateMultiple(pool, LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element, LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Const_StaticElementCount);
        LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* block = (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, next_block_id);
        for (ptrdiff_t i = 0; i < LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Const_StaticElementCount - 1; i++) {
            ObjectPoolSlotPos* next_slot_entry = (ObjectPoolSlotPos*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, block, i);
            next_slot_entry->block_id = next_block_id;
            next_slot_entry->slot_id = i + 1;
        }
        ObjectPoolSlotPos* next_slot = (ObjectPoolSlotPos*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, block, LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Const_StaticElementCount - 1);
        next_slot->block_id = pool->first.block_id;
        next_slot->slot_id = pool->first.slot_id;

        pool->first.block_id = next_block_id;
        pool->first.slot_id = 0;
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
    }
    // 取第一个空闲的slot
    ret_pos->block_id = pool->first.block_id;
    ret_pos->slot_id = pool->first.slot_id;
    LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* next_block = (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, ret_pos->block_id);
    ObjectPoolSlotPos* next_slot = (ObjectPoolSlotPos*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, next_block, ret_pos->slot_id);
    pool->first.block_id = next_slot->block_id;
    pool->first.slot_id = next_slot->slot_id;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
    return (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)next_slot;
}
void ObjectPoolFree(ObjectPool* pool, ObjectPoolSlotPos* free_pos) {
    LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* block = (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, free_pos->block_id);
    ObjectPoolSlotPos* slot = (ObjectPoolSlotPos*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, block, free_pos->slot_id);
    slot->block_id = pool->first.block_id;
    slot->slot_id = pool->first.slot_id;
    pool->first.block_id = free_pos->block_id;
    pool->first.slot_id = free_pos->slot_id;
    free_pos->block_id = LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId;
    free_pos->slot_id = 0;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
}
    
#include <libyuc/space_manager/object_pool.undef>