/*
* @yuyuaqwq - ����
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* �뱣��������
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

// ����
#define CUTILS_SPACE_MANAGER_LIST_BUDDY_TO_POWER_OF_2(exponent) (1 << (exponent))

/*
* ����ʽbuddy��
* logn������ͷ
* λͼ(ÿһ�������Ҫһ��λ�Ŀռ�)
* ����ڵ㣬����Ƕ�뵽ʵ�ʷ��������
* 
* ��ʼ����ֻ����������ͷָ��һ���ڵ�
* 
* λͼʵ���Ϲ���һ��������������ײ�Ľڵ��Ӧ�������������һ��Ľڵ��Ӧ�²�������ӽڵ�
* �ڵ�Ϊ0ʱ��ʾ���Ӷ�δ��������ѱ����䣬Ϊ1ʱ��ʾ����һ�������ѱ����䣬��һ������δ������
* 
* ����ʱ���ȸ��������С��λ����Ӧ������ͷ����������������ժ�´˽ڵ�
* û�оʹӸ��������ͷ��ժ�£�����һ�����䣬��һ���ҵ��¼���������
* 
* �����ݷ����λ�ö�λλͼ���ȡ���׽ڵ㣬�������Ϊ0�����޸�Ϊ1�������޸�Ϊ0
* 
* �ͷ�ʱ
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