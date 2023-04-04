/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_SPACE_MANAGER_BUDDY_H_
#define CUTILS_SPACE_MANAGER_BUDDY_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
* 主要参考自项目：https://github.com/wuwenbin/buddy2
*/

#define LEFT_LEAF(index) ((index) * 2 + 1)
#define RIGHT_LEAF(index) ((index) * 2 + 2)
#define PARENT(index) ( ((index) + 1) / 2 - 1)

#define IS_POWER_OF_2(x) (!((x)&((x)-1)))
#ifndef  MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif // MAX

#define TO_POWER_OF_2(index) (1 << (index))
uint8_t TO_INDEX_OF_2(size_t power) {
	uint8_t index = 0;
	while (power != 0) {
		index++;
		power >> 1;
	}
	return index - 1;
}

typedef struct Buddy {
	uint8_t size;
	uint8_t logn[];
};

static size_t fixsize(size_t size) {
	for (int i = 1; i != sizeof(size) / 2; i *= 2) {
		size |= size >> i;
	}
	return size + 1;
}

bool BuddyInit(Buddy* buddy, size_t size) {
	if (size < 1 || !IS_POWER_OF_2(size)) {
		return false;
	}
	buddy->size = TO_INDEX_OF_2(size);
	size_t node_size = size * 2;

	for (size_t i = 0; i < 2 * size - 1; i++) {
		if (IS_POWER_OF_2(i + 1)) {
			node_size /= 2;
		}
		buddy->logn[i] = TO_INDEX_OF_2(node_size);
	}
	return true;
}

size_t BuddyAlloc(Buddy* buddy, size_t size) {
	if (size == 0) {
		return -1;
	}
	if (!IS_POWER_OF_2(size)) {
		size = fixsize(size);
	}
	if (TO_POWER_OF_2(buddy->logn[0]) < size) {
		return -1;
	}

	// 从二叉树根节点向下找正好符合分配要求的尺寸
	size_t index = 0;
	size_t node_size = TO_POWER_OF_2(buddy->size);
	for (; node_size != size; node_size /= 2) {
		if (buddy->logn[LEFT_LEAF(index)] <= buddy->logn[RIGHT_LEAF(index)]) {
			index = LEFT_LEAF(index);
		}
		else {
			index = RIGHT_LEAF(index);
		}
	}

	// 向上更新节点的logn
	buddy->logn[index] = 0;
	size_t offset = (index + 1) * node_size - TO_POWER_OF_2(buddy->size);
	while (index) {
		index = PARENT(index);
		buddy->logn[index] = MAX(buddy->logn[LEFT_LEAF(index)], buddy->logn[RIGHT_LEAF(index)]);
	}
	return offset;
}

void BuddyFree(Buddy* buddy, size_t offset) {
	  assert(offset < TO_POWER_OF_2(buddy->size));
	uint8_t index = offset + TO_POWER_OF_2(buddy->size) - 1;
	for (; buddy->logn[index]; index = PARENT(index)) {

	}
}

#ifdef __cplusplus
}
#endif

#endif // CUTILS_SPACE_MANAGER_BUDDY_H_