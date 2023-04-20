/*
* @yuyuaqwq - ����
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* �뱣��������
*/

#ifndef CUTILS_SPACE_MANAGER_BUDDY_H_
#define CUTILS_SPACE_MANAGER_BUDDY_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
* ��Ҫ�ο�����Ŀ��https://github.com/wuwenbin/buddy2
*/


#define CUTILS_SPACE_MANAGER_BUDDY_LEFT_LEAF(index) ((index) * 2/* + 1*/)
#define CUTILS_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index) ((index) * 2 + 1/*2*/)
#define CUTILS_SPACE_MANAGER_BUDDY_PARENT(index) ( ((index)/* + 1*/) / 2/* - 1*/)

#define CUTILS_SPACE_MANAGER_BUDDY_IS_POWER_OF_2(x) (!((x)&((x)-1)))
#define CUTILS_SPACE_MANAGER_BUDDY_MAX(a, b) ((a) > (b) ? (a) : (b))

#define CUTILS_SPACE_MANAGER_BUDDY_TO_POWER_OF_2(exponent) (1 << ((exponent)-1))

#define CUTILS_SPACE_MANAGER_BUDDY_DECLARATION(buddy_type_name, id_type) \
	typedef struct _##buddy_type_name##Buddy { \
		/*uint8_t size;*/ \
		uint8_t logn[];		/* ʵ�ʴ��ָ��+1����Ϊ0������ʾû�пռ䣬��2^0Ӧ����1 */ \
	} buddy_type_name##Buddy; \
	buddy_type_name##Buddy* buddy_type_name##BuddyCreate(id_type size); \
	bool buddy_type_name##BuddyInit(buddy_type_name##Buddy* buddy, id_type size); \
	id_type buddy_type_name##BuddyAlloc(buddy_type_name##Buddy* buddy, id_type size); \
	void buddy_type_name##BuddyFree(buddy_type_name##Buddy* buddy, id_type offset); \
	id_type buddy_type_name##BuddyGetMaxFreeCount(buddy_type_name##Buddy* buddy); \
	id_type buddy_type_name##BuddyGetMaxCount(buddy_type_name##Buddy* buddy); \

#define CUTILS_SPACE_MANAGER_BUDDY_DEFINE(buddy_type_name, id_type, indexer, allocator) \
	static uint8_t CUTILS_SPACE_MANAGER_BUDDY_TO_EXPONENT_OF_2(id_type power) { \
		uint8_t exponent = 0; \
		while (power != 0) { \
			exponent++; \
			power >>= 1; \
		} \
		return exponent;  /* - 1 */ \
	} \
	static id_type AlignToPowersOf2(id_type size) { \
		for (int i = 1; i < sizeof(size) * 8 / 2 + 1; i *= 2) { \
			size |= size >> i; \
		} \
		return size + 1; \
	} \
	buddy_type_name##Buddy* buddy_type_name##BuddyCreate(id_type size) { \
		buddy_type_name##Buddy* buddy; \
		id_type alloc_size = size * sizeof(uint8_t) * 2; \
		if (sizeof(id_type) <= 2) { \
			alloc_size /= 2; \
		} \
		buddy = (buddy_type_name##Buddy*)allocator##_CreateMultiple(NULL, uint8_t, alloc_size); \
		return buddy; \
	} \
	bool buddy_type_name##BuddyInit(buddy_type_name##Buddy* buddy, id_type size) { \
		if (size < 1 || !CUTILS_SPACE_MANAGER_BUDDY_IS_POWER_OF_2(size)) { \
			return false; \
		} \
		indexer##_Set(buddy, buddy->logn, 0, CUTILS_SPACE_MANAGER_BUDDY_TO_EXPONENT_OF_2(size)); \
		id_type node_size = size * 2; \
		for (id_type i = 1; i < 2 * size; i++) { \
			if (CUTILS_SPACE_MANAGER_BUDDY_IS_POWER_OF_2(i)) { \
				node_size /= 2; \
			} \
			indexer##_Set(buddy, buddy->logn, i, CUTILS_SPACE_MANAGER_BUDDY_TO_EXPONENT_OF_2(node_size)); \
		} \
		return true; \
	} \
	id_type buddy_type_name##BuddyAlloc(buddy_type_name##Buddy* buddy, id_type size) { \
		if (size == 0) { \
			return -1; \
		} \
		if (!CUTILS_SPACE_MANAGER_BUDDY_IS_POWER_OF_2(size)) { \
			size = AlignToPowersOf2(size); \
		} \
		if (CUTILS_SPACE_MANAGER_BUDDY_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, 1)) < size) { \
			return -1; \
		} \
		/* �Ӷ��������ڵ����������÷��Ϸ���Ҫ��ĳߴ� */ \
		id_type index = 1; \
		id_type node_size = CUTILS_SPACE_MANAGER_BUDDY_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, 0)); \
		id_type size_logn = CUTILS_SPACE_MANAGER_BUDDY_TO_EXPONENT_OF_2(size); \
		for (; node_size != size; node_size /= 2) { \
			/* �����Ҹ�С��ģ��Ͳ��طָ������ */ \
			id_type left_logn = indexer##_Get(buddy, buddy->logn, CUTILS_SPACE_MANAGER_BUDDY_LEFT_LEAF(index)); \
			id_type right_logn = indexer##_Get(buddy, buddy->logn, CUTILS_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index)); \
			if (right_logn < size_logn || left_logn >= size_logn && left_logn <= right_logn) { \
				index = CUTILS_SPACE_MANAGER_BUDDY_LEFT_LEAF(index); \
			} \
			else { \
				index = CUTILS_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index); \
			} \
		} \
		/* ���ϸ��¸��ڵ��logn */ \
		indexer##_Set(buddy, buddy->logn, index, 0); \
		/* ����������ת���ɷ���ƫ�� */ \
		id_type offset = index * node_size - CUTILS_SPACE_MANAGER_BUDDY_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, 0)); \
		while (index != 1) { \
			index = CUTILS_SPACE_MANAGER_BUDDY_PARENT(index); \
			id_type max_logn = CUTILS_SPACE_MANAGER_BUDDY_MAX(indexer##_Get(buddy, buddy->logn, CUTILS_SPACE_MANAGER_BUDDY_LEFT_LEAF(index)), indexer##_Get(buddy, buddy->logn, CUTILS_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index))); \
			if (max_logn == indexer##_Get(buddy, buddy->logn, index)) { \
				break; /*�����￪ʼ����Ӱ���ϲ��logn*/ \
			} \
			indexer##_Set(buddy, buddy->logn, index, max_logn); \
		} \
		return offset; \
	} \
	void buddy_type_name##BuddyFree(buddy_type_name##Buddy* buddy, id_type offset) { \
		  assert(offset != -1 && offset < CUTILS_SPACE_MANAGER_BUDDY_TO_POWER_OF_2(/*buddy->size*/indexer##_Get(buddy, buddy->logn, 0))); \
		id_type node_size_logn = 1; \
		/* ��λ����ײ�Ҷ�ӽڵ㣬�������ҵ�Ϊ0�Ľڵ�(������Ľڵ�) */ \
		id_type index = offset + CUTILS_SPACE_MANAGER_BUDDY_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, 0)); \
		for (; indexer##_Get(buddy, buddy->logn, index); index = CUTILS_SPACE_MANAGER_BUDDY_PARENT(index)) { \
			node_size_logn++; \
			if (index == 1) return; /* û���ҵ�������Ľڵ� */ \
		} \
		/* ���ϸ��¸��ڵ��logn */ \
		indexer##_Set(buddy, buddy->logn, index, node_size_logn); \
		uint8_t left_logn, right_logn; \
		while (index != 1) { \
			index = CUTILS_SPACE_MANAGER_BUDDY_PARENT(index); \
			node_size_logn++; \
			left_logn = indexer##_Get(buddy, buddy->logn, CUTILS_SPACE_MANAGER_BUDDY_LEFT_LEAF(index)); \
			right_logn = indexer##_Get(buddy, buddy->logn, CUTILS_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index)); \
			if (CUTILS_SPACE_MANAGER_BUDDY_TO_POWER_OF_2(left_logn) + CUTILS_SPACE_MANAGER_BUDDY_TO_POWER_OF_2(right_logn) == CUTILS_SPACE_MANAGER_BUDDY_TO_POWER_OF_2(node_size_logn)) { \
				indexer##_Set(buddy, buddy->logn, index, node_size_logn);		/* �ϲ� */ \
			} \
			else { \
				indexer##_Set(buddy, buddy->logn, index, CUTILS_SPACE_MANAGER_BUDDY_MAX(left_logn, right_logn));		/* ��һ��δ���ͷţ��޷��ϲ� */ \
			} \
		} \
	} \
	id_type buddy_type_name##BuddyGetMaxFreeCount(buddy_type_name##Buddy* buddy) { \
		return CUTILS_SPACE_MANAGER_BUDDY_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, 1)); \
	} \
	id_type buddy_type_name##BuddyGetMaxCount(buddy_type_name##Buddy* buddy) { \
		return CUTILS_SPACE_MANAGER_BUDDY_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, 0)); \
	} \

#define CUTILS_SPACE_MANAGER_BUDDY_4BIT_INDEXER_Get(BUDDY, LOGN, INDEX) ((INDEX) % 2 ? (LOGN)[(INDEX) / 2] & 0xf : (LOGN)[(INDEX) / 2] >> 4)
#define CUTILS_SPACE_MANAGER_BUDDY_4BIT_INDEXER_Set(BUDDY, LOGN, INDEX, NEW_LOGN) ((LOGN)[(INDEX) / 2] = ((INDEX) % 2) ? ((NEW_LOGN) | ((LOGN)[(INDEX) / 2] & 0xf0)) : ((NEW_LOGN) << 4 | ((LOGN)[(INDEX) / 2] & 0xf)))
#define CUTILS_SPACE_MANAGER_BUDDY_4BIT_INDEXER CUTILS_SPACE_MANAGER_BUDDY_4BIT_INDEXER


#ifdef __cplusplus
}
#endif

#endif // CUTILS_SPACE_MANAGER_BUDDY_H_