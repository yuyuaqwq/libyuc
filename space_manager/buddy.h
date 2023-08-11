/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_SPACE_MANAGER_BUDDY_H_
#define LIBYUC_SPACE_MANAGER_BUDDY_H_

#include <libyuc/basic.h>
#include <libyuc/algorithm/two.h>
#include <libyuc/container/experimental/cb_tree.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
* 主要参考自项目：https://github.com/wuwenbin/buddy2
*/


#define LIBYUC_SPACE_MANAGER_BUDDY_PARENT(index) LIBYUC_CONTAINER_CB_TREE_ONE_GET_PARENT(index)
#define LIBYUC_SPACE_MANAGER_BUDDY_LEFT_LEAF(index) LIBYUC_CONTAINER_CB_TREE_ONE_GET_LEFT(index)
#define LIBYUC_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index) LIBYUC_CONTAINER_CB_TREE_ONE_GET_RIGHT(index)


#define LIBYUC_SPACE_MANAGER_BUDDY_DECLARATION(buddy_type_name, id_type) \
    typedef struct _##buddy_type_name##Buddy { \
        /*uint8_t size;*/ \
        uint8_t logn[];        /* 实际存放指数+1，因为0用来表示没有空间，而2^0应该是1 */ \
    } buddy_type_name##Buddy; \
    buddy_type_name##Buddy* buddy_type_name##BuddyCreate(id_type size); \
    bool buddy_type_name##BuddyInit(buddy_type_name##Buddy* buddy, id_type size); \
    id_type buddy_type_name##BuddyAlloc(buddy_type_name##Buddy* buddy, id_type size); \
    bool buddy_type_name##BuddyAllocByOffset(buddy_type_name##Buddy* buddy, id_type offset, id_type size); \
    id_type buddy_type_name##BuddyFree(buddy_type_name##Buddy* buddy, id_type offset); \
    id_type buddy_type_name##BuddyGetAllocBlockSize(buddy_type_name##Buddy* buddy, id_type offset); \
    id_type buddy_type_name##BuddyGetMaxFreeCount(buddy_type_name##Buddy* buddy); \
    id_type buddy_type_name##BuddyGetMaxCount(buddy_type_name##Buddy* buddy); \

#define LIBYUC_SPACE_MANAGER_BUDDY_DEFINE(buddy_type_name, id_type, indexer, allocator) \
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
        if (size < 1 || !LIBYUC_ALGORITHM_TWO_IS_POWER_OF_2(size)) { \
            return false; \
        } \
        indexer##_Set(buddy, buddy->logn, 0, LIBYUC_ALGORITHM_TWO_TO_EXPONENT_OF_2(size) + 1); \
        id_type node_size = size * 2; \
        for (id_type i = 1; i < 2 * size; i++) { \
            if (LIBYUC_ALGORITHM_TWO_IS_POWER_OF_2(i)) { \
                node_size /= 2; \
            } \
            indexer##_Set(buddy, buddy->logn, i, LIBYUC_ALGORITHM_TWO_TO_EXPONENT_OF_2(node_size) + 1); \
        } \
        return true; \
    } \
    id_type buddy_type_name##BuddyAlloc(buddy_type_name##Buddy* buddy, id_type size) { \
        if (size == 0) { \
            return -1; \
        } \
        if (!LIBYUC_ALGORITHM_TWO_IS_POWER_OF_2(size)) { \
            size = LIBYUC_ALGORITHM_TWO_ALIGN_TO_POWER_OF_2(size); \
        } \
        if (LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, 1)-1) < size) { \
            return -1; \
        } \
        /* 从二叉树根节点向下找正好符合分配要求的尺寸 */ \
        id_type index = 1; \
        id_type node_size = LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, 0)-1); \
        id_type size_logn = LIBYUC_ALGORITHM_TWO_TO_EXPONENT_OF_2(size) + 1; \
        for (; node_size != size; node_size /= 2) { \
            /* 优先找更小块的，就不必分割大块的了 */ \
            id_type left_logn = indexer##_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_LEFT_LEAF(index)); \
            id_type right_logn = indexer##_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index)); \
            if (right_logn < size_logn || left_logn >= size_logn && left_logn <= right_logn) { \
                index = LIBYUC_SPACE_MANAGER_BUDDY_LEFT_LEAF(index); \
            } \
            else { \
                index = LIBYUC_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index); \
            } \
        } \
        /* 向上更新父节点的logn */ \
        indexer##_Set(buddy, buddy->logn, index, 0); \
        /* 二叉树索引转换成分配偏移 */ \
        id_type offset = index * node_size - LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, 0)-1); \
        while (index != 1) { \
            index = LIBYUC_SPACE_MANAGER_BUDDY_PARENT(index); \
            id_type max_logn = max(indexer##_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_LEFT_LEAF(index)), indexer##_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index))); \
            if (max_logn == indexer##_Get(buddy, buddy->logn, index)) { \
                break; /*从这里开始不会影响上层的logn*/ \
            } \
            indexer##_Set(buddy, buddy->logn, index, max_logn); \
        } \
        return offset; \
    } \
    bool buddy_type_name##BuddyAllocByOffset(buddy_type_name##Buddy* buddy, id_type offset, id_type size) { \
         assert(offset != -1 && offset < LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(/*buddy->size*/indexer##_Get(buddy, buddy->logn, 0)-1)); \
        id_type node_size = 1; \
        /* 定位到最底层叶子节点，并向上找到匹配size的节点 */ \
        id_type index = offset + LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, 0)-1); \
        while (size < node_size) { \
            node_size *= 2; \
            index = LIBYUC_SPACE_MANAGER_BUDDY_PARENT(index); \
            if (index == 1) return false; /* 大小无法匹配 */ \
        } \
        if (size != node_size) return false; \
        if (LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, index)) != node_size) return false; \
        /* 向上更新父节点的logn */ \
        indexer##_Set(buddy, buddy->logn, index, 0); \
        while (index != 1) { \
            index = LIBYUC_SPACE_MANAGER_BUDDY_PARENT(index); \
            id_type max_logn = max(indexer##_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_LEFT_LEAF(index)), indexer##_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index))); \
            if (max_logn == indexer##_Get(buddy, buddy->logn, index)) { \
                break; /*从这里开始不会影响上层的logn*/ \
            } \
            indexer##_Set(buddy, buddy->logn, index, max_logn); \
        } \
        return true; \
    } \
    id_type buddy_type_name##BuddyFree(buddy_type_name##Buddy* buddy, id_type offset) { \
         assert(offset != -1 && offset < LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(/*buddy->size*/indexer##_Get(buddy, buddy->logn, 0)-1)); \
        id_type node_size_logn = 1; \
        /* 定位到最底层叶子节点，并向上找到为0的节点(被分配的节点) */ \
        id_type index = offset + LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, 0)-1); \
        for (; indexer##_Get(buddy, buddy->logn, index); index = LIBYUC_SPACE_MANAGER_BUDDY_PARENT(index)) { \
            node_size_logn++; \
            if (index == 1) return 0; /* 没有找到被分配的节点 */ \
        } \
        id_type alloc_aize = LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(node_size_logn-1); \
        /* 向上更新父节点的logn */ \
        indexer##_Set(buddy, buddy->logn, index, node_size_logn); \
        uint8_t left_logn, right_logn; \
        while (index != 1) { \
            index = LIBYUC_SPACE_MANAGER_BUDDY_PARENT(index); \
            node_size_logn++; \
            left_logn = indexer##_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_LEFT_LEAF(index)); \
            right_logn = indexer##_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index)); \
            if (LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(left_logn-1) + LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(right_logn-1) == LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(node_size_logn-1)) { \
                indexer##_Set(buddy, buddy->logn, index, node_size_logn);        /* 合并 */ \
            } \
            else { \
                indexer##_Set(buddy, buddy->logn, index, max(left_logn, right_logn));        /* 另一侧未被释放，无法合并 */ \
            } \
        } \
        return alloc_aize; \
    } \
    id_type buddy_type_name##BuddyGetAllocBlockSize(buddy_type_name##Buddy* buddy, id_type offset) { \
            assert(offset != -1 && offset < LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(/*buddy->size*/indexer##_Get(buddy, buddy->logn, 0)-1)); \
        id_type node_size_logn = 1; \
        /* 定位到最底层叶子节点，并向上找到为0的节点(被分配的节点) */ \
        id_type index = offset + LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, 0)-1); \
        for (; indexer##_Get(buddy, buddy->logn, index); index = LIBYUC_SPACE_MANAGER_BUDDY_PARENT(index)) { \
            node_size_logn++; \
            if (index == 1) return -1; /* 没有找到被分配的节点 */ \
        } \
        return LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(node_size_logn-1); \
    } \
    id_type buddy_type_name##BuddyGetMaxFreeCount(buddy_type_name##Buddy* buddy) { \
        return LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, 1)-1); \
    } \
    id_type buddy_type_name##BuddyGetMaxCount(buddy_type_name##Buddy* buddy) { \
        return LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(indexer##_Get(buddy, buddy->logn, 0)-1); \
    } \

#define LIBYUC_SPACE_MANAGER_BUDDY_4BIT_INDEXER_Get(BUDDY, LOGN, INDEX) ((INDEX) % 2 ? (LOGN)[(INDEX) / 2] & 0xf : (LOGN)[(INDEX) / 2] >> 4)
#define LIBYUC_SPACE_MANAGER_BUDDY_4BIT_INDEXER_Set(BUDDY, LOGN, INDEX, NEW_LOGN) ((LOGN)[(INDEX) / 2] = ((INDEX) % 2) ? ((NEW_LOGN) | ((LOGN)[(INDEX) / 2] & 0xf0)) : ((NEW_LOGN) << 4 | ((LOGN)[(INDEX) / 2] & 0xf)))
#define LIBYUC_SPACE_MANAGER_BUDDY_4BIT_INDEXER LIBYUC_SPACE_MANAGER_BUDDY_4BIT_INDEXER

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_SPACE_MANAGER_BUDDY_H_