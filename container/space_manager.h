/*
* @yuyuaqwq - ����
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* �뱣��������
*/

#ifndef CUTILS_CONTAINER_SPACE_MANAGER_H_
#define CUTILS_CONTAINER_SPACE_MANAGER_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* �ռ����
*/
#define CUTILS_CONTAINER_SPACE_MANAGER_DECLARATION(space_manager_type_name, id_type, list_count) \
	typedef struct _##space_manager_type_name##SpaceBlock { \
		id_type next_block_offset; \
		id_type len; \
	} space_manager_type_name##SpaceBlock; \
	typedef struct _##space_manager_type_name##SpaceHead { \
		id_type first_block[list_count];		/* �ֱ�ָ��ͬ���еĵ�һ�����п� */ \
		/* uint8_t space[]; */ \
	} space_manager_type_name##SpaceHead; \

#define CUTILS_CONTAINER_SPACE_MANAGER_DEFINE(space_manager_type_name, id_type, referencer, list_count) \
	/*
	* ��ʼ��
	*/ \
	void space_manager_type_name##SpaceManagerInit(space_manager_type_name##SpaceHead* head, id_type space_size) { \
		head->first_block[0] = sizeof(space_manager_type_name##SpaceHead); \
		for (int16_t i = 1; i < list_count; i++) { \
			head->first_block[i] = referencer##_InvalidId; \
		} \
		space_manager_type_name##SpaceBlock* block = (space_manager_type_name##SpaceBlock*)((uintptr_t)head + sizeof(space_manager_type_name##SpaceHead)); \
		block->next_block_offset = referencer##_InvalidId; \
		block->len = space_size - sizeof(space_manager_type_name##SpaceHead); \
	} \
	/*
	* ����飬����ƫ��
	*/ \
	id_type space_manager_type_name##SpaceManagerAlloc(space_manager_type_name##SpaceHead* head, id_type list_order, id_type len) { \
		space_manager_type_name##SpaceBlock* prev_block = (space_manager_type_name##SpaceBlock*)((uintptr_t)&head->first_block[list_order]); \
		\
		id_type free_offset = head->first_block[list_order]; \
		while (free_offset != referencer##_InvalidId) { \
			space_manager_type_name##SpaceBlock* block = (space_manager_type_name##SpaceBlock*)((uintptr_t)head + free_offset); \
			if (block->len > len) { \
				void* mem = (void*)(&block->next_block_offset); \
				space_manager_type_name##SpaceBlock* new_block = (space_manager_type_name##SpaceBlock*)((uintptr_t)head + free_offset + len); \
				new_block->next_block_offset = block->next_block_offset; \
				new_block->len = block->len - len; \
				prev_block->next_block_offset += len; \
				return free_offset; \
			} \
			else if (block->len == len) { \
				prev_block->next_block_offset = block->next_block_offset; \
				return free_offset; \
			} \
			free_offset = block->next_block_offset; \
			prev_block = block; \
		}; \
		return referencer##_InvalidId; \
	} \
	/*
	* �ͷſ�
	*/ \
	void space_manager_type_name##SpaceManagerFree(space_manager_type_name##SpaceHead* head, id_type list_order, id_type free_offset, id_type len) { \
		id_type cur_offset = head->first_block[list_order]; \
		space_manager_type_name##SpaceBlock* prev_block = (space_manager_type_name##SpaceBlock*)((uintptr_t)&head->first_block[list_order]); \
		\
		space_manager_type_name##SpaceBlock* cur_block = (space_manager_type_name##SpaceBlock*)((uintptr_t)head + free_offset); \
		/* ���Ժϲ��뵱ǰ��������ǰ����п� */ \
		bool prev = false, next = false; \
		while (cur_offset != referencer##_InvalidId) { \
			space_manager_type_name##SpaceBlock* cur_block = (space_manager_type_name##SpaceBlock*)((uintptr_t)head + cur_offset); \
			if (!next && free_offset + len == cur_offset) { \
				/* �ҵ������Ŀ�����һ�� */ \
				len += cur_block->len; \
				cur_block->len = len; \
				cur_block->next_block_offset = cur_block->next_block_offset; \
				prev_block->next_block_offset = free_offset; \
				cur_block = cur_block; \
				next = true; \
			} \
			else if (!prev && cur_offset + cur_block->len == free_offset) { \
				/* �ҵ������Ŀ�����һ�� */ \
				free_offset = cur_offset; \
				len += cur_block->len;\
				cur_block->len = len; \
				cur_block = cur_block; \
				prev_block->next_block_offset = free_offset; \
				prev = true; \
			} \
			else { \
				/* û�кϲ��Ÿ��£��ҵ������Ŀ�����һ��Ҳ����������߼��Ƚϸ���
				 ��ҪΪ��ʹ����һ��ѭ�����ҵ������Ŀ�����һ��ʱ��prev�ܹ���ȷ��ָ��ϲ����free_block */ \
				prev_block = cur_block; \
			} \
			if (prev && next) break; \
			cur_offset = cur_block->next_block_offset; \
		} \
		if (!prev && !next) { \
			cur_block->next_block_offset = head->first_block[list_order]; \
			cur_block->len = len; \
			head->first_block[list_order] = free_offset; \
		} \
	} \
	/*
	* ��ѯ���鳤��
	*/ \
	id_type space_manager_type_name##SpaceManagerGetMaxFreeBlockSize(space_manager_type_name##SpaceHead* head, id_type list_order) { \
		space_manager_type_name##SpaceBlock* prev_block = (space_manager_type_name##SpaceBlock*)((uintptr_t)&head->first_block[list_order]); \
		id_type free_offset = head->first_block[list_order]; \
		id_type max = 0; \
		while (free_offset != referencer##_InvalidId) { \
			space_manager_type_name##SpaceBlock* block = (space_manager_type_name##SpaceBlock*)((uintptr_t)head + free_offset); \
			if (block->len > max) { \
				max = block->len; \
			} \
		} \
		return max; \
	} \



#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_SPACE_MANAGER_H_