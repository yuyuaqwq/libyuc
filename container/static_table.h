/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/


#ifndef CUTILS_CONTAINER_STATIC_TABLE_H_
#define CUTILS_CONTAINER_STATIC_TABLE_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif //  __cplusplus

typedef struct _StaticBlock {
	uint16_t next_block_offset;
	uint16_t len;
} StaticBlock;

typedef struct _StaticTable {
	uint16_t first_block[1];		// 分别指向不同队列的第一个空闲块
	// uint8_t data[];
} StaticTable;

extern const uint16_t kStaticBlockInvalidOffset;

void StaticTableInit(StaticTable* table, int16_t list_count, int16_t head_size, int16_t block_size);
uint16_t StaticTablePop(StaticTable* table, uint16_t list_order, uint16_t len);
void StaticTablePush(StaticTable* page, uint16_t list_order, uint16_t free_offset, uint16_t len);
int16_t StaticTableGetMaxBlockSize(StaticTable* table, uint16_t list_order);

#ifdef __cplusplus
}
#endif //  __cplusplus

#endif // CUTILS_CONTAINER_STATIC_TABLE_H_