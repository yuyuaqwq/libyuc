#include "CUtils/container/static_table.h"


const uint16_t kStaticBlockInvalidOffset = 0;


/*
* 初始化
*/
void StaticTableInit(StaticTable* table, int16_t list_count, int16_t head_size, int16_t block_size) {
	table->first_block[0] = head_size;
	for (int16_t i = 1; i < list_count; i++) {
		table->first_block[i] = kStaticBlockInvalidOffset;
	}
	StaticBlock* block = (StaticBlock*)((uintptr_t)table + head_size);
	block->next_block_offset = kStaticBlockInvalidOffset;
	block->len = block_size - head_size;
}

/*
* 分配块，返回偏移
*/
uint16_t StaticTablePop(StaticTable* table, uint16_t list_order, uint16_t len) {
	StaticBlock* prev_block = (StaticBlock*)((uintptr_t)&table->first_block[list_order]);

	uint16_t free_offset = table->first_block[list_order];
	while (free_offset != kStaticBlockInvalidOffset) {
		StaticBlock* block = (StaticBlock*)((uintptr_t)table + free_offset);
		if (block->len > len) {
			void* mem = (void*)(&block->next_block_offset);
			StaticBlock* new_block = (StaticBlock*)((uintptr_t)table + free_offset + len);
			new_block->next_block_offset = block->next_block_offset;
			new_block->len = block->len - len;
			prev_block->next_block_offset += len;
			return free_offset;
		}
		else if (block->len == len) {
			prev_block->next_block_offset = block->next_block_offset;
			return free_offset;
		}
		free_offset = block->next_block_offset;
		prev_block = block;
	};
	return kStaticBlockInvalidOffset;
}

/*
* 释放块
*/
void StaticTablePush(StaticTable* table, uint16_t list_order, uint16_t free_offset, uint16_t len) {
	uint16_t cur_offset = table->first_block[list_order];
	StaticBlock* prev_block = (StaticBlock*)((uintptr_t)&table->first_block[list_order]);

	StaticBlock* cur_block = (StaticBlock*)((uintptr_t)table + free_offset);
	// 尝试合并与当前块连续的前后空闲块
	bool prev = false, next = false;
	
	while (cur_offset != kStaticBlockInvalidOffset) {
		StaticBlock* cur_block = (StaticBlock*)((uintptr_t)table + cur_offset);
		if (!next && free_offset + len == cur_offset) {
			// 找到连续的空闲下一块
			len += cur_block->len;
			cur_block->len = len;
			cur_block->next_block_offset = cur_block->next_block_offset;
			prev_block->next_block_offset = free_offset;
			cur_block = cur_block;
			next = true;
		}
		else if (!prev && cur_offset + cur_block->len == free_offset) {
			// 找到连续的空闲上一块
			free_offset = cur_offset;
			len += cur_block->len;
			cur_block->len = len;
			cur_block = cur_block;
			prev_block->next_block_offset = free_offset;
			prev = true;
		}
		else {
			// 没有合并才更新，找到连续的空闲下一块也不更新这块逻辑比较复杂
			// 主要为了使得下一次循环若找到连续的空闲上一块时，prev能够正确的指向合并后的free_block
			prev_block = cur_block;
		}
		if (prev && next) break;
		cur_offset = cur_block->next_block_offset;
	}
	if (!prev && !next) {
		cur_block->next_block_offset = table->first_block[list_order];
		cur_block->len = len;
		table->first_block[list_order] = free_offset;
	}
}

/*
* 查询最大块长度
*/
int16_t StaticTableGetMaxBlockSize(StaticTable* table, uint16_t list_order) {
	StaticBlock* prev_block = (StaticBlock*)((uintptr_t)&table->first_block[list_order]);
	uint16_t free_offset = table->first_block[list_order];
	uint16_t max = 0;
	while (free_offset != kStaticBlockInvalidOffset) {
		StaticBlock* block = (StaticBlock*)((uintptr_t)table + free_offset);
		if (block->len > max) {
			max = block->len;
		}
	}
	return max;
}


