/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/


#ifndef LIBYUC_CONTAINER_RADIX_TREE_H_
#define LIBYUC_CONTAINER_RADIX_TREE_H_

#include <libyuc/basic.h>

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
            RadixValue value;        // 为内部节点时则会存在空间浪费
        };
        struct _RadixEntry* down;
    };
} RadixSlot;

typedef struct _RadixEntry {
    struct _RadixEntry* parent;
    int16_t count;
    uint8_t slot_status[LIBYUC_CONTAINER_RADIX_TREE_MAP_BIT_TO_BYTE_COUNT];        // 1为kv，0为下级指针
    RadixSlot slots[LIBYUC_CONTAINER_RADIX_TREE_MAP_SIZE];
} RadixEntry;

typedef struct _RadixTree {
    RadixEntry* root;
} RadixTree;


void RadixTreeInit(RadixTree* tree);
RadixValue* RadixTreeFind(RadixTree* tree, RadixKey key);
bool RadixTreeInsert(RadixTree* tree, RadixKey key, RadixValue value);
bool RadixTreeDelete(RadixTree* tree, RadixKey key);

inline static bool RadixGetSlotType(RadixEntry* entry, int32_t index) {
    int32_t byte_pos = index / 8;
    int32_t bit_pos = index % 8;
    return (RadixSlotType)(entry->slot_status[byte_pos] & (1 << bit_pos));
}

inline static void RadixSetSlotType(RadixEntry* entry, int32_t index, bool is_kv) {
    int32_t byte_pos = index / 8;
    int32_t bit_pos = index % 8;
    if (is_kv) {
        entry->slot_status[byte_pos] |= 1 << bit_pos;
    }
    else {
        entry->slot_status[byte_pos] &= ~(1 << bit_pos);
    }
}

inline static int32_t RadixGetSlotIndex(RadixKey key, int32_t level) {
    return (key >> (LIBYUC_CONTAINER_RADIX_TREE_MAP_LEVEL - (level + 1)) * LIBYUC_CONTAINER_RADIX_TREE_MAP_SHIFT) & LIBYUC_CONTAINER_RADIX_TREE_MAP_MASK;
}

forceinline static RadixSlot* RadixTreeFindSlot(RadixTree* tree, RadixKey key, RadixEntry** entry, int32_t* ret_slot_index, int32_t* level) {
    RadixEntry* cur_entry = tree->root;
    for (int32_t i = 0; cur_entry && i < LIBYUC_CONTAINER_RADIX_TREE_MAP_LEVEL; i++) {
        int32_t slot_index = RadixGetSlotIndex(key, i);
        if (ret_slot_index) { *ret_slot_index = slot_index; }
        if (entry) { *entry = cur_entry; }
        if (level) { *level = i; }
        if (RadixGetSlotType(cur_entry, slot_index)) {
            return &cur_entry->slots[slot_index];
        }
        cur_entry = cur_entry->slots[slot_index].down;
    }
    return NULL;
}


void RadixEntryInit(RadixEntry* entry, RadixEntry* parent) {
    for (int32_t i = 0; i < LIBYUC_CONTAINER_RADIX_TREE_MAP_SIZE; i++) {
        entry->slots[i].down = NULL;
    }
    for (int32_t i = 0; i < LIBYUC_CONTAINER_RADIX_TREE_MAP_BIT_TO_BYTE_COUNT; i++) {
        entry->slot_status[i] = 0;
    }
    entry->parent = parent;
    entry->count = 0;
}

void RadixTreeInit(RadixTree* tree) {
    tree->root = ObjectCreate(RadixEntry);
    RadixEntryInit(tree->root, NULL);
}

static void RadixTreeRecursiveRelease(RadixTree* tree, RadixEntry* entry) {
    if (entry == NULL) {
        return;
    }
    for (int32_t i = 0; i < LIBYUC_CONTAINER_RADIX_TREE_MAP_SIZE; i++) {
        if (!RadixGetSlotType(entry, i)) {
            RadixTreeRecursiveRelease(tree, entry->slots[i].down);
        }
    }
}

void RadixTreeRelease(RadixTree* tree) {
    RadixTreeRecursiveRelease(tree, tree->root);
}

RadixValue* RadixTreeFind(RadixTree* tree, RadixKey key) {
    RadixSlot* slot = RadixTreeFindSlot(tree, key, NULL, NULL, NULL);
    if (!slot) {
        return NULL;
    }
    if (slot->key == key) {
        return &slot->value;
    }
    return NULL;
}

bool RadixTreeInsert(RadixTree* tree, RadixKey key, RadixValue value) {
    RadixEntry* entry;
    int32_t slot_index;
    int32_t level;
    RadixSlot* slot = RadixTreeFindSlot(tree, key, &entry, &slot_index, &level);
    if (!slot) {
        // 空槽，直接插入kv
        RadixSetSlotType(entry, slot_index, true);
        entry->slots[slot_index].key = key;
        entry->slots[slot_index].value = value;
        ++entry->count;
        return true;
    }

    // 是kv
    if (slot->key == key) {
        slot->value = value;
        return true;
    }

    int32_t new_slot_index, old_slot_index = slot_index;
    RadixEntry* down_entry, * cur_entry = entry;

    RadixKey old_key = slot->key;
    RadixKey old_Value = slot->value;
    // 存在key冲突，需要持续向下分裂
    do {
        level++;
        RadixSetSlotType(cur_entry, old_slot_index, false);
        down_entry = ObjectCreate(RadixEntry);
        cur_entry->slots[old_slot_index].down = down_entry;
        ++cur_entry->count;
        RadixEntryInit(down_entry, cur_entry);

        old_slot_index = RadixGetSlotIndex(old_key, level);
        new_slot_index = RadixGetSlotIndex(key, level);
        if (new_slot_index != old_slot_index) {
            // 在新的slots中不冲突，插入结束
            RadixSetSlotType(down_entry, new_slot_index, true);
            down_entry->slots[new_slot_index].key = key;
            down_entry->slots[new_slot_index].value = value;

            // 原先的kv也移到个槽里
            RadixSetSlotType(down_entry, old_slot_index, true);
            down_entry->slots[old_slot_index].key = old_key;
            down_entry->slots[old_slot_index].value = old_Value;

            down_entry->count += 2;
            break;
        }
        // 继续向下创建
        cur_entry = down_entry;
    } while (true);

    return true;
}

bool RadixTreeDelete(RadixTree* tree, RadixKey key) {
    RadixEntry* entry;
    int32_t slot_index;
    int32_t level;
    RadixSlot* slot = RadixTreeFindSlot(tree, key, &entry, &slot_index, &level);
    if (!slot) {
        return false;
    }
    do {
        entry->slots[slot_index].down = NULL;
        RadixSetSlotType(entry, slot_index, false);
        --entry->count;
        if (entry->count > 0) {
            break;
        }
        RadixEntry* temp = entry->parent;
        ObjectDelete(entry);
        entry = temp;
        slot_index = RadixGetSlotIndex(key, --level);
    } while (entry);
    return true;
}


#endif // LIBYUC_CONTAINER_REDIX_TREE_H_