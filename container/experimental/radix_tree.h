/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/


#ifndef CUTILS_CONTAINER_RADIX_TREE_H_
#define CUTILS_CONTAINER_RADIX_TREE_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 基数树
*/

typedef uint32_t RadixKey;
typedef uintptr_t RadixValue;

#define CUTILS_CONTAINER_RADIX_TREE_KEY_BIT_COUNT (sizeof(RadixKey) * 8)

#define CUTILS_CONTAINER_RADIX_TREE_MAP_SHIFT 4
#define CUTILS_CONTAINER_RADIX_TREE_MAP_MASK ((1 << CUTILS_CONTAINER_RADIX_TREE_MAP_SHIFT) - 1)
#define CUTILS_CONTAINER_RADIX_TREE_MAP_SIZE (1 << CUTILS_CONTAINER_RADIX_TREE_MAP_SHIFT)
#define CUTILS_CONTAINER_RADIX_TREE_MAP_LEVEL (CUTILS_CONTAINER_RADIX_TREE_KEY_BIT_COUNT / CUTILS_CONTAINER_RADIX_TREE_MAP_SHIFT + (CUTILS_CONTAINER_RADIX_TREE_KEY_BIT_COUNT % CUTILS_CONTAINER_RADIX_TREE_MAP_SHIFT ? 1 : 0))

#define CUTILS_CONTAINER_RADIX_TREE_MAP_BIT_TO_BYTE_COUNT (CUTILS_CONTAINER_RADIX_TREE_MAP_SIZE / 8 + (CUTILS_CONTAINER_RADIX_TREE_MAP_SIZE % 8 ? 1 : 0) )

typedef enum _RadixSlotType {
	kRadixSlotValue,
	kRadixSlotDownPtr,
} RadixSlotType;

typedef struct _RadixSlot {
	union {
		struct {
			RadixKey key;
			RadixValue value;		// 为内部节点时则会存在空间浪费
		};
		struct _RadixEntry* down;
	};
} RadixSlot;

typedef struct _RadixEntry {
	struct _RadixEntry* parent;
	int16_t count;
	uint8_t slot_status[CUTILS_CONTAINER_RADIX_TREE_MAP_BIT_TO_BYTE_COUNT];		// 1为kv，0为下级指针
	RadixSlot slots[CUTILS_CONTAINER_RADIX_TREE_MAP_SIZE];
} RadixEntry;

typedef struct _RadixTree {
	RadixEntry* root;
} RadixTree;


void RadixTreeInit(RadixTree* tree);
RadixValue* RadixTreeFind(RadixTree* tree, RadixKey key);
bool RadixTreeInsert(RadixTree* tree, RadixKey key, RadixValue value);
bool RadixTreeDelete(RadixTree* tree, RadixKey key);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_REDIX_TREE_H_