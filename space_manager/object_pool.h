/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_SPACE_MANAGER_OBJECT_POOL_H_
#define CUTILS_SPACE_MANAGER_OBJECT_POOL_H_

#include <CUtils/object.h>
#include <CUtils/container/hash_table.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 动态对象池
*/


#define CUTILS_SPACE_MANAGER_OBJECT_POOL_DECLARATION(object_pool_name, obj_type, bucket_id_type, index_id_type) \
	typedef struct _##object_pool_name##ObjectPoolBucketEntry { \
		bucket_id_type bucket_id; /* 下一个entry的bucket_id */ \
		index_id_type index; /* 下一个entry的index */ \
	} object_pool_name##ObjectPoolBucketEntry; \
	typedef struct _##object_pool_name##ObjectPool { \
		object_pool_name##ObjectPoolBucketEntry first_entry; \
	} object_pool_name##ObjectPool; \
	CUTILS_CONTAINER_HASH_TABLE_DECLARATION(object_pool_name##ObjectPoolBucket, bucket_id_type, bucket_id_type); \
	void object_pool_name##ObjectPoolBucketEntryInit(object_pool_name##ObjectPoolBucketEntry* entry); \
	void object_pool_name##ObjectPoolInit(object_pool_name##ObjectPool* pool); \
	void object_pool_name##ObjectPoolRelease(object_pool_name##ObjectPool* pool); \
	obj_type* object_pool_name##ObjectPoolAlloc(object_pool_name##ObjectPool* pool, object_pool_name##ObjectPoolBucketEntry* ret_entry); \
	void object_pool_name##ObjectPoolFree(object_pool_name##ObjectPool* pool, object_pool_name##ObjectPoolBucketEntry* free_entry); \

#define CUTILS_SPACE_MANAGER_OBJECT_POOL_DEFINE(object_pool_name, obj_type, bucket_id_type, index_id_type, bucket_accessor, bucket_allocator, bucket_referencer, indexer) \
	CUTILS_CONTAINER_HASH_TABLE_DEFINE(object_pool_name##ObjectPoolBucket, bucket_id_type, bucket_id_type, CUTILS_OBJECT_ALLOCATOR_DEFALUT, CUTILS_CONTAINER_HASH_TABLE_DEFAULT_ACCESSOR, CUTILS_OBJECT_MOVER_DEFALUT, CUTILS_CONTAINER_HASH_TABLE_DEFAULT_HASHER, CUTILS_OBJECT_COMPARER_DEFALUT) \
	void object_pool_name##ObjectPoolBucketEntryInit(object_pool_name##ObjectPoolBucketEntry* entry) { \
		entry->bucket_id = bucket_referencer##_InvalidId; \
		entry->index = 0; \
	} \
	void object_pool_name##ObjectPoolInit(object_pool_name##ObjectPool* pool) { \
		object_pool_name##ObjectPoolBucketEntryInit(&pool->first_entry); \
	} \
	/*
	* 将所有分配的对象释放后，再调用该函数释放整个对象池，否则可能出现内存泄露
	*/ \
	void object_pool_name##ObjectPoolRelease(object_pool_name##ObjectPool* pool) { \
		if (pool->first_entry.bucket_id == bucket_referencer##_InvalidId) return; \
		bucket_id_type bucket_id = pool->first_entry.bucket_id; \
		index_id_type index = pool->first_entry.index; \
		object_pool_name##ObjectPoolBucketHashTable table; \
		object_pool_name##ObjectPoolBucketHashTableInit(&table, 0, 0); \
		while (bucket_id != bucket_referencer##_InvalidId) { \
			obj_type* bucket = (obj_type*)bucket_referencer##_Reference(pool, bucket_id); \
			object_pool_name##ObjectPoolBucketEntry* bucket_entry = (object_pool_name##ObjectPoolBucketEntry*)&indexer##_Get(pool, &bucket, index); \
			object_pool_name##ObjectPoolBucketHashTablePut(&table, &bucket_id); \
			bucket_id = bucket_entry->bucket_id; \
			index = bucket_entry->index; \
			bucket_referencer##_Dereference(pool, bucket); \
		} \
		object_pool_name##ObjectPoolBucketHashTableIterator iter; \
        bucket_id_type* bucket_id_ptr = object_pool_name##ObjectPoolBucketHashTableIteratorFirst(&table, &iter); \
        while (bucket_id_ptr) { \
			bucket_allocator##_Release(pool, *bucket_id_ptr); \
			bucket_id_ptr = object_pool_name##ObjectPoolBucketHashTableIteratorNext(&table, &iter); \
        } \
		object_pool_name##ObjectPoolBucketHashTableRelease(&table); \
		pool->first_entry.bucket_id = bucket_referencer##_InvalidId; \
		pool->first_entry.index = 0; \
	} \
	/*
	* 为便于使用，会直接返回被解引用后的内存地址，若不是内存对象池请注意返回地址是否可用
	*/ \
	obj_type* object_pool_name##ObjectPoolAlloc(object_pool_name##ObjectPool* pool, object_pool_name##ObjectPoolBucketEntry* ret_entry) { \
		if (pool->first_entry.bucket_id == bucket_referencer##_InvalidId) { \
			bucket_id_type bucket_id = (bucket_id_type)bucket_allocator##_CreateMultiple(pool, obj_type, bucket_accessor##_GetMaxCount(pool)); \
			obj_type* bucket = (obj_type*)bucket_referencer##_Reference(pool, bucket_id); \
			for (ptrdiff_t i = 0; i < bucket_accessor##_GetMaxCount(pool) - 1; i++) { \
				object_pool_name##ObjectPoolBucketEntry* bucket_entry = (object_pool_name##ObjectPoolBucketEntry*)&indexer##_Get(pool, &bucket, i); \
				bucket_entry->bucket_id = bucket_id; \
				bucket_entry->index = i + 1; \
			} \
			object_pool_name##ObjectPoolBucketEntry* bucket_entry = (object_pool_name##ObjectPoolBucketEntry*)&indexer##_Get(pool, &bucket, bucket_accessor##_GetMaxCount() - 1); \
			bucket_entry->bucket_id = pool->first_entry.bucket_id; \
			bucket_entry->index = pool->first_entry.index; \
			\
			pool->first_entry.bucket_id = bucket_id; \
			pool->first_entry.index = 0; \
			bucket_referencer##_Dereference(pool, bucket); \
		} \
		ret_entry->bucket_id = pool->first_entry.bucket_id; \
		ret_entry->index = pool->first_entry.index; \
		obj_type* bucket = (obj_type*)bucket_referencer##_Reference(pool, ret_entry->bucket_id); \
		object_pool_name##ObjectPoolBucketEntry* bucket_entry = (object_pool_name##ObjectPoolBucketEntry*)&indexer##_Get(pool, &bucket, ret_entry->index); \
		pool->first_entry.bucket_id = bucket_entry->bucket_id; \
		pool->first_entry.index = bucket_entry->index; \
		bucket_referencer##_Dereference(pool, bucket); \
		return (obj_type*)bucket_entry; \
	} \
	void object_pool_name##ObjectPoolFree(object_pool_name##ObjectPool* pool, object_pool_name##ObjectPoolBucketEntry* free_entry) { \
		obj_type* bucket = (obj_type*)bucket_referencer##_Reference(pool, free_entry->bucket_id); \
		object_pool_name##ObjectPoolBucketEntry* bucket_entry = (object_pool_name##ObjectPoolBucketEntry*)&indexer##_Get(pool, &bucket, free_entry->index); \
		bucket_entry->bucket_id = pool->first_entry.bucket_id; \
		bucket_entry->index = pool->first_entry.index; \
		pool->first_entry.bucket_id = free_entry->bucket_id; \
		pool->first_entry.index = free_entry->index; \
		free_entry->bucket_id = bucket_referencer##_InvalidId; \
		free_entry->index = 0; \
		bucket_referencer##_Dereference(pool, bucket); \
	} \

#define CUTILS_SPACE_MANAGER_OBJECT_POOL_DEFAULT_ACCESSOR_GetMaxCount(pool) 512
#define CUTILS_SPACE_MANAGER_OBJECT_POOL_DEFAULT_ACCESSOR CUTILS_SPACE_MANAGER_OBJECT_POOL_DEFAULT_ACCESSOR

//CUTILS_SPACE_MANAGER_OBJECT_POOL_DECLARATION(Int, int64_t, struct _IntObjectPoolBucketEntry*, int16_t)
//CUTILS_SPACE_MANAGER_OBJECT_POOL_DEFINE(Int, int64_t, struct _IntObjectPoolBucketEntry*, int16_t, CUTILS_SPACE_MANAGER_OBJECT_POOL_DEFAULT_ACCESSOR, CUTILS_OBJECT_ALLOCATOR_DEFALUT, CUTILS_OBJECT_REFERENCER_DEFALUT)

#ifdef __cplusplus
}
#endif

#endif // CUTILS_SPACE_MANAGER_OBJECT_POOL_H_