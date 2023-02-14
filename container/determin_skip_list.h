/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_DETERMIN_SKIP_LIST_H_
#define CUTILS_CONTAINER_DETERMIN_SKIP_LIST_H_

#include <CUtils/object.h>
#include <CUtils/container/static_list.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
* 1-2-3确定性跳跃表
* 等价于234树，基于预分裂和预合并的操作，即可实现自顶向下插入与删除
*/

typedef struct _DeterminSkipEntry {
	struct _DeterminSkipEntry* next;
} DeterminSkipEntry;

typedef struct _DeterminSkipIndex {
	union {
		StaticListEntry free_entry;
		struct {
			struct _DeterminSkipIndex* next;
			struct _DeterminSkipIndex* down;
		};
	};
	
} DeterminSkipIndex;

typedef struct _DeterminSkipList {
	StaticList index_pool;		// 实现内存分配器提高性能(索引节点固定长度，静态链表实现O(1)分配释放)
	DeterminSkipIndex* start;
	DeterminSkipEntry* first;
	size_t count;
	int entry_field_offset;
	int key_field_offset;
	int key_size;
	CmpFunc cmp_func;
} DeterminSkipList;


void DeterminSkipListInit(DeterminSkipList* list, int entry_field_offset, int key_field_offset, int key_size, CmpFunc cmp_func) {
	StaticListInit(&list->index_pool, 8, sizeof(DeterminSkipIndex), 0);
	list->start = NULL;
	list->first = NULL;
	list->count = 0;
	list->entry_field_offset = entry_field_offset;
	list->key_field_offset = key_field_offset;
	list->key_size = key_size;
	if (!cmp_func) {
		cmp_func = MemoryCmpR;
	}
	list->cmp_func = cmp_func;
}

void DeterminSkipListFind(DeterminSkipList* list) {

}

void DeterminSkipListInsert(DeterminSkipList* list, void* entry) {
	void* obj = GetObjByFieldOffset(entry, list->entry_field_offset, void);
	void* key = GetFieldByFieldOffset(obj, list->key_field_offset, void);

}


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_DETERMIN_SKIP_LIST_H_