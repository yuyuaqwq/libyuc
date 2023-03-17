/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_SPACE_MANAGER_H_
#define CUTILS_CONTAINER_SPACE_MANAGER_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 空间管理
*/
#define CUTILS_CONTAINER_SPACE_MANAGER_DECLARATION(space_manager_type_name, id_type, list_count) \
	typedef struct _##space_manager_type_name##SpaceBlock { \
		id_type next_block_offset; \
		id_type len; \
	} space_manager_type_name##SpaceBlock; \
	typedef struct _##space_manager_type_name##SpaceHead { \
		id_type first_block[list_count];		/* 分别指向不同队列的第一个空闲块 */ \
		/* uint8_t space[]; */ \
	} space_manager_type_name##SpaceHead; \

#define CUTILS_CONTAINER_SPACE_MANAGER_DEFINE(space_manager_type_name, id_type, referencer, list_count) \
	/*
	* 初始化
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
	* 分配块，返回偏移
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
	* 释放块
	*/ \
	void space_manager_type_name##SpaceManagerFree(space_manager_type_name##SpaceHead* head, id_type list_order, id_type free_offset, id_type len) { \
		id_type cur_offset = head->first_block[list_order]; \
		space_manager_type_name##SpaceBlock* prev_block = (space_manager_type_name##SpaceBlock*)((uintptr_t)&head->first_block[list_order]); \
		\
		space_manager_type_name##SpaceBlock* cur_block = (space_manager_type_name##SpaceBlock*)((uintptr_t)head + free_offset); \
		/* 尝试合并与当前块连续的前后空闲块 */ \
		bool prev = false, next = false; \
		while (cur_offset != referencer##_InvalidId) { \
			space_manager_type_name##SpaceBlock* cur_block = (space_manager_type_name##SpaceBlock*)((uintptr_t)head + cur_offset); \
			if (!next && free_offset + len == cur_offset) { \
				/* 找到连续的空闲下一块 */ \
				len += cur_block->len; \
				cur_block->len = len; \
				cur_block->next_block_offset = cur_block->next_block_offset; \
				prev_block->next_block_offset = free_offset; \
				cur_block = cur_block; \
				next = true; \
			} \
			else if (!prev && cur_offset + cur_block->len == free_offset) { \
				/* 找到连续的空闲上一块 */ \
				free_offset = cur_offset; \
				len += cur_block->len;\
				cur_block->len = len; \
				cur_block = cur_block; \
				prev_block->next_block_offset = free_offset; \
				prev = true; \
			} \
			else { \
				/* 没有合并才更新，找到连续的空闲下一块也不更新这块逻辑比较复杂
				 主要为了使得下一次循环若找到连续的空闲上一块时，prev能够正确的指向合并后的free_block */ \
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
	* 查询最大块长度
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