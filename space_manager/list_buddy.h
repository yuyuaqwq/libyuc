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
* logn个链表头
* 位图(每一个项就需要一个位的空间)
* 链表节点，可以嵌入到实际分配的数据
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
* 释放时
*/



#define CUTILS_SPACE_MANAGER_LIST_BUDDY_DECLARATION(list_buddy_type_name, id_type, list_entry_id_type) \
	typedef struct _##list_buddy_type_name##ListBuddyEntry { \
		list_entry_id_type next; \
	} list_buddy_type_name##ListBuddyEntry; \
	typedef struct _##list_buddy_type_name##ListBuddy { \
		list_buddy_type_name##ListBuddyEntry list_head[];	 \
		/* Bitmap status; */ \
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
		buddy->list_head[i] = ; \
	} \
	


#ifdef __cplusplus
}
#endif

#endif // CUTILS_SPACE_MANAGER_LIST_BUDDY_H_