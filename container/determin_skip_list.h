/*
* @yuyuaqwq - ����
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* �뱣��������
*/

#ifndef CUTILS_CONTAINER_DETERMIN_SKIP_LIST_H_
#define CUTILS_CONTAINER_DETERMIN_SKIP_LIST_H_

#include <CUtils/object.h>
#include <CUtils/container/static_list.h>
#include <CUtils/container/list.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
* 1-2-3ȷ������Ծ��
* �ȼ���234��������Ԥ���Ѻ�Ԥ�ϲ��Ĳ���������ʵ���Զ����²�����ɾ��
*/

typedef struct _DeterminSkipEntry {
	ListEntry list_entry;
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
	StaticList index_pool;		// ʵ���ڴ�������������(�����ڵ�̶����ȣ���̬����ʵ��O(1)�����ͷ�)
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



/*
 
head       4
     1     4   6       10
     1 2 3 4 5 6 7 8 9 10

	 
	       4
	  1           6        10
	    2 3    5    7 8 9     


��϶����1-3��
*/
void DeterminSkipListFind(DeterminSkipList* list) {

}

void DeterminSkipListInsert(DeterminSkipList* list, void* entry) {
	void* obj = ObjectGetFromFieldOffset(entry, list->entry_field_offset, void);
	void* key = ObjectGetFieldByOffset(obj, list->key_field_offset, void);

}


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_DETERMIN_SKIP_LIST_H_