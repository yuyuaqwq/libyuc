/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/


#ifndef LIBYUC_CONTAINER_RADIX_TREE_H_
#define LIBYUC_CONTAINER_RADIX_TREE_H_

#include <libyuc/object.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 基数树
*/

typedef uint32_t RadixKey;
typedef uintptr_t RadixValue;

#define LIBYUC_CONTAINER_RADIX_TREE_KEY_BIT_COUNT (sizeof(RadixKey) * 8)

#define LIBYUC_CONTAINER_RADIX_TREE_MAP_SHIFT 4
#define LIBYUC_CONTAINER_RADIX_TREE_MAP_MASK ((1 << LIBYUC_CONTAINER_RADIX_TREE_MAP_SHIFT) - 1)
#define LIBYUC_CONTAINER_RADIX_TREE_MAP_SIZE (1 << LIBYUC_CONTAINER_RADIX_TREE_MAP_SHIFT)
#define LIBYUC_CONTAINER_RADIX_TREE_MAP_LEVEL (LIBYUC_CONTAINER_RADIX_TREE_KEY_BIT_COUNT / LIBYUC_CONTAINER_RADIX_TREE_MAP_SHIFT + (LIBYUC_CONTAINER_RADIX_TREE_KEY_BIT_COUNT % LIBYUC_CONTAINER_RADIX_TREE_MAP_SHIFT ? 1 : 0))

#define LIBYUC_CONTAINER_RADIX_TREE_MAP_BIT_TO_BYTE_COUNT (LIBYUC_CONTAINER_RADIX_TREE_MAP_SIZE / 8 + (LIBYUC_CONTAINER_RADIX_TREE_MAP_SIZE % 8 ? 1 : 0) )

typedef enum _RadixSlotType {
  kRadixSlotValue,
  kRadixSlotDownPtr,
} RadixSlotType;

typedef struct _RadixSlot {
  union {
    struct {
      RadixKey key;
      RadixValue value;    // 为内部节点时则会存在空间浪费
    };
    struct _RadixEntry* down;
  };
} RadixSlot;

typedef struct _RadixEntry {
  struct _RadixEntry* parent;
  int16_t count;
  uint8_t slot_status[LIBYUC_CONTAINER_RADIX_TREE_MAP_BIT_TO_BYTE_COUNT];    // 1为kv，0为下级指针
  RadixSlot slots[LIBYUC_CONTAINER_RADIX_TREE_MAP_SIZE];
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

#endif // LIBYUC_CONTAINER_REDIX_TREE_H_