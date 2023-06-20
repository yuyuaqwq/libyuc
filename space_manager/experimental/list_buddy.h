/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_SPACE_MANAGER_LIST_BUDDY_H_
#define CUTILS_SPACE_MANAGER_LIST_BUDDY_H_

#include <CUtils/object.h>
#include <CUtils/container/experimental/cb_tree.h>
#include <CUtils/container/singly_list.h>
#include <CUtils/container/bitmap.h>

#ifdef __cplusplus
extern "C" {
#endif


#define CUTILS_SPACE_MANAGER_LIST_BUDDY_PARENT(index) CUTILS_CONTAINER_CB_TREE_ONE_GET_PARENT(index)
#define CUTILS_SPACE_MANAGER_LIST_BUDDY_LEFT_LEAF(index) CUTILS_CONTAINER_CB_TREE_ONE_GET_LEFT(index)
#define CUTILS_SPACE_MANAGER_LIST_BUDDY_RIGHT_LEAF(index) CUTILS_CONTAINER_CB_TREE_ONE_GET_RIGHT(index)

#define CUTILS_SPACE_MANAGER_LIST_BUDDY_IS_POWER_OF_2(x) (!((x)&((x)-1)))
#define CUTILS_SPACE_MANAGER_LIST_BUDDY_MAX(a, b) ((a) > (b) ? (a) : (b))

// 求幂
#define CUTILS_SPACE_MANAGER_LIST_BUDDY_TO_POWER_OF_2(exponent) (1 << (exponent))

/*
* 链表式buddy：
* logn个双向链表头(快速删除)
* 位图(每一个项就需要一个位的空间，可以嵌入到实际分配的空间但不可被覆盖，考虑提供两种模式，一种位图集一种内联到结构中)
* 链表节点，可以嵌入到实际分配的空间(被分配后可以被覆盖)
* 
* 初始化后只有最大的链表头指向一个节点
* 
* 位图实际上构成一个满二叉树，最底层的节点对应两个连续的项，上一层的节点对应下层的左右子节点
* 节点为0时表示孩子都未被分配或都已被分配，为1时表示其中一个孩子已被分配，另一个孩子未被分配
* 
* 分配时首先根据需求大小定位到对应的链表头，如果有则从链表中摘下此节点
* 没有就从更大的链表头中摘下，其中一个分配，另一个挂到下级的链表中
* 
* 随后根据分配的位置定位位图项，获取父亲节点，如果父亲为0则将其修改为1，否则修改为0
* 
* 释放时如果父亲为0则将其修改为1，否则修改为0，并找到兄弟节点对应的空闲链表节点(链表节点内嵌到实际分配的空间中，因此通过计算可以定位)，将其从当前链表摘下，随后选择将左子节点挂到大一级的链表
*/



#define CUTILS_SPACE_MANAGER_LIST_BUDDY_DECLARATION(list_buddy_type_name, id_type, list_entry_id_type) \
	typedef struct _##list_buddy_type_name##ListBuddyEntry { \
		list_entry_id_type next; \
	} list_buddy_type_name##ListBuddyEntry; \
	typedef struct _##list_buddy_type_name##ListBuddy { \
		list_buddy_type_name##ListBuddyEntry list_head[];  \
		/* uint8_t space[]; */ \
	} list_buddy_type_name##ListBuddy; \
	list_buddy_type_name##ListBuddy* list_buddy_type_name##ListBuddyCreate(id_type size); \
	bool list_buddy_type_name##ListBuddyInit(list_buddy_type_name##ListBuddy* buddy, id_type size); \
	id_type list_buddy_type_name##ListBuddyAlloc(list_buddy_type_name##ListBuddy* buddy, id_type size); \
	void list_buddy_type_name##ListBuddyFree(list_buddy_type_name##ListBuddy* buddy, id_type offset); \
	id_type list_buddy_type_name##ListBuddyGetAllocBlockSize(list_buddy_type_name##ListBuddy* buddy, id_type offset); \
	id_type list_buddy_type_name##ListBuddyGetMaxFreeCount(list_buddy_type_name##ListBuddy* buddy); \
	id_type list_buddy_type_name##ListBuddyGetMaxCount(list_buddy_type_name##ListBuddy* buddy); \

#define CUTILS_SPACE_MANAGER_LIST_BUDDY_DEFINE(list_buddy_type_name, id_type, indexer, allocator, referencer) \
	void list_buddy_type_name##ListBuddyInit(list_buddy_type_name##ListBuddy* buddy, id_type size) { \
		id_type i = 0; \
		do { \
			size /= 2; \
			if (size == 0) { \
				break; \
			} \
			buddy->list_head[i] = referencer##_InvalidId; \
		} while (true); \
		buddy->list_head[i] = 0; \
	} \
	\
	



#define list_entry_id_type int32_t
#define id_type int32_t
#define list_count




CUTILS_CONTAINER_SINGLY_LIST_DECLARATION(ListBuddy, list_entry_id_type)

typedef struct _ListBuddyObj {
	struct _ListBuddy* buddy;
	ListBuddySinglyListHead list_head;
} ListBuddyObj;

typedef struct _ListBuddy {
	uint8_t logn;		// 实际存储指数+1
	ListBuddySinglyListHead list_head[];
	/* uint8_t space[]; */
} ListBuddy;

forceinline ListBuddySinglyListEntry* CUTILS_SPACE_MANAGER_LIST_BUDDY_SINGLY_LIST_REFERENCER_Reference(ListBuddySinglyListHead* list_head, list_entry_id_type id) {
	ListBuddyObj* obj = ObjectGetFromField(list_head, ListBuddyObj, list_head);
	return (ListBuddySinglyListEntry*)((uintptr_t)obj->buddy + id);
}
#define CUTILS_SPACE_MANAGER_LIST_BUDDY_SINGLY_LIST_REFERENCER_Dereference(MAIN_OBJ, OBJ)
#define CUTILS_SPACE_MANAGER_LIST_BUDDY_SINGLY_LIST_REFERENCER_InvalidId (-1)
#define CUTILS_SPACE_MANAGER_LIST_BUDDY_SINGLY_LIST_REFERENCER CUTILS_SPACE_MANAGER_LIST_BUDDY_SINGLY_LIST_REFERENCER

CUTILS_CONTAINER_SINGLY_LIST_DEFINE(ListBuddy, list_entry_id_type, CUTILS_SPACE_MANAGER_LIST_BUDDY_SINGLY_LIST_REFERENCER)


static id_type ListBuddyAlignToPowersOf2(id_type size) {
	for (int i = 1; i < sizeof(size) * 8 / 2 + 1; i *= 2) {
		size |= size >> i;
	}
	return size + 1;
}

void ListBuddyInit(ListBuddy* buddy, id_type size) {
	if (size == 0) {
		buddy->logn = 0;
		return;
	}
	id_type head_size = sizeof(ListBuddy) + sizeof(ListBuddySinglyListHead) * buddy->logn;
	if (!CUTILS_SPACE_MANAGER_LIST_BUDDY_IS_POWER_OF_2(head_size)) {
		head_size = ListBuddyAlignToPowersOf2(head_size);
	}
	size -= head_size;
	buddy->logn = 0;
	id_type mask = 1 << (sizeof(id_type) * 8 - 1);
	do {
		buddy->logn++;
		size /= 2;
		if (size == 0) {
			break;
		}
		if (mask & size) {

		}
		else {
			buddy->list_head[buddy->logn - 1].first = -1;
		}
		mask >>= 1;
	} while (true);
	
	buddy->list_head[buddy->logn-1].first = head_size;
	
	((ListBuddySinglyListEntry*)((uintptr_t)buddy + buddy->list_head[buddy->logn - 1].first))->next = -1;
}

id_type ListBuddyAlloc(ListBuddy* buddy, size_t size) {
	if (size == 0) {
		return -1; 
	} 
	if (!CUTILS_SPACE_MANAGER_LIST_BUDDY_IS_POWER_OF_2(size)) {
		size = ListBuddyAlignToPowersOf2(size);
	}
	size_t cur_size = 1;
	int i = 0;
	for (; i < buddy->logn; i++) {
		if (cur_size == size) {
			break;
		}
		cur_size *= 2;
	}
	if (i == buddy->logn) {
		return -1;
	}
	ListBuddyObj obj;
	obj.buddy = buddy;
	list_entry_id_type entry;
	if (buddy->list_head[i].first == -1) {
		// 当前链表不存在可分配的节点，需要从更大的链表摘下并分裂
		int j = i + 1;
		for (; j < buddy->logn; j++) {
			cur_size *= 2;
			if (buddy->list_head[j].first != -1) {
				break;
			}
		}
		if (j == buddy->logn) {
			return -1;
		}
		obj.list_head = buddy->list_head[j];
		entry = ListBuddySinglyListDeleteFirst(&obj.list_head);
		buddy->list_head[j] = obj.list_head;
		while (j > i) {
			--j;
			cur_size /= 2;
			list_entry_id_type mid = entry + cur_size;		// 后半部分放到下级链表
			obj.list_head = buddy->list_head[j];
			ListBuddySinglyListPutFirst(&obj.list_head, mid);
			buddy->list_head[j] = obj.list_head;
		}
	}
	else {
		obj.list_head = buddy->list_head[i];
		entry = ListBuddySinglyListDeleteFirst(&obj.list_head);
		buddy->list_head[i] = obj.list_head;
	}

}


#ifdef __cplusplus
}
#endif

#endif // CUTILS_SPACE_MANAGER_LIST_BUDDY_H_