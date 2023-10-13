#include <libyuc/space_manager/object_pool.def>

/*
* 优化思路：
* 为pool分配一张block表，每个元素存放指向实际block的指针(可以动态扩容)，block_id是表中的索引
*/

#define LIBYUC_CONTAINER_HASH_TABLE_CLASS_NAME ObjectPoolBlock
#define LIBYUC_CONTAINER_HASH_TABLE_INDEXER_Type_Element uintptr_t
#define LIBYUC_CONTAINER_HASH_TABLE_HASHER_HashCode(main_obj, obj) (_wymix(*(obj), UINT64_C(0x9E3779B97F4A7C15)))
#include <libyuc/container/hash_table.c>

void ObjectPoolSlotInit(ObjectPoolSlot* entry) {
    entry->next_block_id = LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId;
    entry->next_slot_index = 0;
}
void ObjectPoolInit(ObjectPool* pool) {
    ObjectPoolSlotInit(&pool->first_slot);
}
/*
* 将所有分配的对象释放后，再调用该函数释放整个对象池，否则可能出现内存泄露
* 通过block表则不需要释放整个对象池
*/
void ObjectPoolRelease(ObjectPool* pool) {
    if (pool->first_slot.next_block_id == LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId) return;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id next_block_id = pool->first_slot.next_block_id;
    LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Id next_slot_index = pool->first_slot.next_slot_index;
    ObjectPoolBlockHashTable table;
    ObjectPoolBlockHashTableInit(&table, 0, 0);
    while (next_block_id != LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId) {
        LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* bucket = (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, next_block_id);
        ObjectPoolSlot* bucket_entry = (ObjectPoolSlot*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, &bucket, next_slot_index);
        ObjectPoolBlockHashTablePut(&table, (const uintptr_t*)&next_block_id);
        next_block_id = bucket_entry->next_block_id;
        next_slot_index = bucket_entry->next_slot_index;
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
    }
    ObjectPoolBlockHashTableIterator iter;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id* next_block_id_ptr = (LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id*)ObjectPoolBlockHashTableIteratorFirst(&table, &iter);
    while (next_block_id_ptr) {
        LIBYUC_CONTAINER_OBJECT_POOL_ALLOCATOR_Release(pool, *next_block_id_ptr);
        next_block_id_ptr = (LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id*)ObjectPoolBlockHashTableIteratorNext(&table, &iter);
    }
    ObjectPoolBlockHashTableRelease(&table);
    pool->first_slot.next_block_id = LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId;
    pool->first_slot.next_slot_index = 0;
}
/*
* 为便于使用，会直接返回被解引用后的内存地址，若不是内存对象池请注意返回地址是否可用
*/
LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* ObjectPoolAlloc(ObjectPool* pool, ObjectPoolSlot* ret_slot) {
    if (pool->first_slot.next_block_id == LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId) {
        // 没有空闲的块
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id next_block_id = (LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Type_Id)LIBYUC_CONTAINER_OBJECT_POOL_ALLOCATOR_CreateMultiple(pool, LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element, LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Const_StaticElementCount);
        LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* block = (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, next_block_id);
        for (ptrdiff_t i = 0; i < LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Const_StaticElementCount - 1; i++) {
            ObjectPoolSlot* bucket_entry = (ObjectPoolSlot*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, block, i);
            bucket_entry->next_block_id = next_block_id;
            bucket_entry->next_slot_index = i + 1;
        }
        ObjectPoolSlot* bucket_entry = (ObjectPoolSlot*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, block, LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Const_StaticElementCount - 1);
        bucket_entry->next_block_id = pool->first_slot.next_block_id;
        bucket_entry->next_slot_index = pool->first_slot.next_slot_index;

        pool->first_slot.next_block_id = next_block_id;
        pool->first_slot.next_slot_index = 0;
        LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
    }
    // 取第一个空闲的slot
    ret_slot->next_block_id = pool->first_slot.next_block_id;
    ret_slot->next_slot_index = pool->first_slot.next_slot_index;
    LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* next_block = (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, ret_slot->next_block_id);
    ObjectPoolSlot* next_slot = (ObjectPoolSlot*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, next_block, ret_slot->next_slot_index);
    pool->first_slot.next_block_id = next_slot->next_block_id;
    pool->first_slot.next_slot_index = next_slot->next_slot_index;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
    return (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)next_slot;
}
void ObjectPoolFree(ObjectPool* pool, ObjectPoolSlot* free_slot) {
    LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element* block = (LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_Type_Element*)LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Reference(pool, free_slot->next_block_id);
    ObjectPoolSlot* bucket_entry = (ObjectPoolSlot*)LIBYUC_CONTAINER_OBJECT_POOL_INDEXER_GetPtr(pool, block, free_slot->next_slot_index);
    bucket_entry->next_block_id = pool->first_slot.next_block_id;
    bucket_entry->next_slot_index = pool->first_slot.next_slot_index;
    pool->first_slot.next_block_id = free_slot->next_block_id;
    pool->first_slot.next_slot_index = free_slot->next_slot_index;
    free_slot->next_block_id = LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Const_InvalidId;
    free_slot->next_slot_index = 0;
    LIBYUC_SPACE_MANAGER_OBJECT_POOL_REFERENCER_Dereference(pool, bucket);
}
    
#include <libyuc/space_manager/object_pool.undef>