/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>
#include <libyuc/algorithm/two.h>
#include <libyuc/container/experimental/cb_tree.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
* 主要参考自项目：https://github.com/wuwenbin/buddy2
*/


#define LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_4BIT_Get(BUDDY, LOGN, INDEX) ((INDEX) % 2 ? (LOGN)[(INDEX) / 2] & 0xf : (LOGN)[(INDEX) / 2] >> 4)
#define LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_4BIT_Set(BUDDY, LOGN, INDEX, NEW_LOGN) ((LOGN)[(INDEX) / 2] = ((INDEX) % 2) ? ((NEW_LOGN) | ((LOGN)[(INDEX) / 2] & 0xf0)) : ((NEW_LOGN) << 4 | ((LOGN)[(INDEX) / 2] & 0xf)))


#ifndef LIBYUC_SPACE_MANAGER_BUDDY_CLASS_NAME
#define LIBYUC_SPACE_MANAGER_BUDDY_CLASS_NAME
#endif

#ifndef LIBYUC_SPACE_MANAGER_BUDDY_ACCESSOR_GetArray
#define LIBYUC_SPACE_MANAGER_BUDDY_ACCESSOR_GetArray(main_obj) ((obj)->obj_arr)
#endif

#ifndef LIBYUC_SPACE_MANAGER_BUDDY_ALLOCATOR_Create
#define LIBYUC_SPACE_MANAGER_BUDDY_ALLOCATOR_Create(main_obj, obj_type) MemoryAlloc(sizeof(obj_type))
#endif
#ifndef LIBYUC_SPACE_MANAGER_BUDDY_ALLOCATOR_CreateMultiple
#define LIBYUC_SPACE_MANAGER_BUDDY_ALLOCATOR_CreateMultiple(main_obj, obj_type, count) ((obj_type*)MemoryAlloc(sizeof(obj_type) * count))
#endif
#ifndef LIBYUC_SPACE_MANAGER_BUDDY_ALLOCATOR_Release
#define LIBYUC_SPACE_MANAGER_BUDDY_ALLOCATOR_Release(main_obj, obj) MemoryFree(obj)
#endif

#ifndef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_DynamicArray
#define LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_DynamicArray LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Element*
#endif
#ifndef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Const_StaticElementCount
#define LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Const_StaticElementCount 1
#endif
#ifndef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Const_InvalidId
#define LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Const_InvalidId (-1)
#endif
#ifndef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Const_InvalidDynamicArray
#define LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Const_InvalidDynamicArray (NULL)
#endif
#ifndef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id
#define LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id size_t
#endif
#ifndef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset
#define LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id
#endif
#ifndef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Set
#define LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Set(BUDDY, LOGN, INDEX, NEW) ((LOGN)[(INDEX)] = NEW)
#endif
#ifndef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get
#define LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(BUDDY, LOGN, INDEX) ((LOGN)[(INDEX)])
#endif


#define LIBYUC_SPACE_MANAGER_BUDDY_PARENT(index) LIBYUC_CONTAINER_CB_TREE_ONE_GET_PARENT(index)
#define LIBYUC_SPACE_MANAGER_BUDDY_LEFT_LEAF(index) LIBYUC_CONTAINER_CB_TREE_ONE_GET_LEFT(index)
#define LIBYUC_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index) LIBYUC_CONTAINER_CB_TREE_ONE_GET_RIGHT(index)

#define Buddy MAKE_NAME(LIBYUC_SPACE_MANAGER_BUDDY_CLASS_NAME, Buddy)

typedef struct Buddy {
    /*uint8_t size;*/
    uint8_t logn[];        /* 实际存放指数+1，因为0用来表示没有空间，而2^0应该是1 */
} Buddy;

#define BuddyCreate MAKE_NAME(LIBYUC_SPACE_MANAGER_BUDDY_CLASS_NAME, BuddyCreate)
#define BuddyInit MAKE_NAME(LIBYUC_SPACE_MANAGER_BUDDY_CLASS_NAME, BuddyInit)
#define BuddyAlloc MAKE_NAME(LIBYUC_SPACE_MANAGER_BUDDY_CLASS_NAME, BuddyAlloc)
#define BuddyAllocByOffset MAKE_NAME(LIBYUC_SPACE_MANAGER_BUDDY_CLASS_NAME, BuddyAllocByOffset)
#define BuddyFree MAKE_NAME(LIBYUC_SPACE_MANAGER_BUDDY_CLASS_NAME, BuddyFree)
#define BuddyGetAllocBlockSize MAKE_NAME(LIBYUC_SPACE_MANAGER_BUDDY_CLASS_NAME, BuddyGetAllocBlockSize)
#define BuddyGetMaxFreeCount MAKE_NAME(LIBYUC_SPACE_MANAGER_BUDDY_CLASS_NAME, BuddyGetMaxFreeCount)
#define BuddyGetMaxCount MAKE_NAME(LIBYUC_SPACE_MANAGER_BUDDY_CLASS_NAME, BuddyGetMaxCount)


Buddy* BuddyCreate(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset size);
bool BuddyInit(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset size);
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id BuddyAlloc(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset size);
bool BuddyAllocByOffset(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id offset, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset size);
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset BuddyFree(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id offset);
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset BuddyGetAllocBlockSize(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id offset);
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset BuddyGetMaxFreeCount(Buddy* buddy);
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset BuddyGetMaxCount(Buddy* buddy);


#ifdef LIBYUC_TEMPLATE_DEFINE
Buddy* BuddyCreate(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset size) {
    Buddy* buddy;
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset alloc_size = size * sizeof(uint8_t) * 2;
    if (sizeof(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset) <= 2) {
        alloc_size /= 2;
    }
    buddy = (Buddy*)LIBYUC_SPACE_MANAGER_BUDDY_ALLOCATOR_CreateMultiple(NULL, uint8_t, alloc_size);
    return buddy;
}
bool BuddyInit(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset size) {
    if (size < 1 || !LIBYUC_ALGORITHM_TWO_IS_POWER_OF_2(size)) {
        return false;
    }
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Set(buddy, buddy->logn, 0, LIBYUC_ALGORITHM_TWO_TO_EXPONENT_OF_2(size) + 1);
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset node_size = size * 2;
    for (LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id i = 1; i < 2 * size; i++) {
        if (LIBYUC_ALGORITHM_TWO_IS_POWER_OF_2(i)) {
            node_size /= 2;
        }
        LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Set(buddy, buddy->logn, i, LIBYUC_ALGORITHM_TWO_TO_EXPONENT_OF_2(node_size) + 1);
    }
    return true;
}
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id BuddyAlloc(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset size) {
    if (size == 0) {
        return -1;
    }
    if (!LIBYUC_ALGORITHM_TWO_IS_POWER_OF_2(size)) {
        size = LIBYUC_ALGORITHM_TWO_ALIGN_TO_POWER_OF_2(size);
    }
    if (LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, 1)-1) < size) {
        return -1;
    }
    /* 从二叉树根节点向下找正好符合分配要求的尺寸 */
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id index = 1;
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset node_size = LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, 0)-1);
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id size_logn = LIBYUC_ALGORITHM_TWO_TO_EXPONENT_OF_2(size) + 1;
    for (; node_size != size; node_size /= 2) {
        /* 优先找更小块的，就不必分割大块的了 */
        LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id left_logn = LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_LEFT_LEAF(index));
        LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id right_logn = LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index));
        if (right_logn < size_logn || left_logn >= size_logn && left_logn <= right_logn) {
            index = LIBYUC_SPACE_MANAGER_BUDDY_LEFT_LEAF(index);
        }
        else {
            index = LIBYUC_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index);
        }
    }
    /* 向上更新父节点的logn */
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Set(buddy, buddy->logn, index, 0);
    /* 二叉树索引转换成分配偏移 */
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id offset = index * node_size - LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, 0)-1);
    while (index != 1) {
        index = LIBYUC_SPACE_MANAGER_BUDDY_PARENT(index);
        LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id max_logn = max(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_LEFT_LEAF(index)), LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index)));
        if (max_logn == LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, index)) {
            break; /*从这里开始不会影响上层的logn*/
        }
        LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Set(buddy, buddy->logn, index, max_logn);
    }
    return offset;
}
bool BuddyAllocByOffset(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id offset, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset size) {
        assert(offset != -1 && offset < LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(/*buddy->size*/LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, 0)-1));
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset node_size = 1;
    /* 定位到最底层叶子节点，并向上找到匹配size的节点 */
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id index = offset + LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, 0)-1);
    while (size < node_size) {
        node_size *= 2;
        index = LIBYUC_SPACE_MANAGER_BUDDY_PARENT(index);
        if (index == 1) return false; /* 大小无法匹配 */
    }
    if (size != node_size) return false;
    if (LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, index)) != node_size) return false;
    /* 向上更新父节点的logn */
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Set(buddy, buddy->logn, index, 0);
    while (index != 1) {
        index = LIBYUC_SPACE_MANAGER_BUDDY_PARENT(index);
        LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id max_logn = max(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_LEFT_LEAF(index)), LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index)));
        if (max_logn == LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, index)) {
            break; /*从这里开始不会影响上层的logn*/
        }
        LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Set(buddy, buddy->logn, index, max_logn);
    }
    return true;
}
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset BuddyFree(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id offset) {
        assert(offset != -1 && offset < LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(/*buddy->size*/LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, 0)-1));
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset node_size_logn = 1;
    /* 定位到最底层叶子节点，并向上找到为0的节点(被分配的节点) */
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id index = offset + LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, 0)-1);
    for (; LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, index); index = LIBYUC_SPACE_MANAGER_BUDDY_PARENT(index)) {
        node_size_logn++;
        if (index == 1) return 0; /* 没有找到被分配的节点 */
    }
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id alloc_aize = LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(node_size_logn-1);
    /* 向上更新父节点的logn */
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Set(buddy, buddy->logn, index, node_size_logn);
    uint8_t left_logn, right_logn;
    while (index != 1) {
        index = LIBYUC_SPACE_MANAGER_BUDDY_PARENT(index);
        node_size_logn++;
        left_logn = LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_LEFT_LEAF(index));
        right_logn = LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, LIBYUC_SPACE_MANAGER_BUDDY_RIGHT_LEAF(index));
        if (LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(left_logn-1) + LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(right_logn-1) == LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(node_size_logn-1)) {
            LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Set(buddy, buddy->logn, index, node_size_logn);        /* 合并 */
        }
        else {
            LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Set(buddy, buddy->logn, index, max(left_logn, right_logn));        /* 另一侧未被释放，无法合并 */
        }
    }
    return alloc_aize;
}
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset BuddyGetAllocBlockSize(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id offset) {
        assert(offset != -1 && offset < LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(/*buddy->size*/LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, 0)-1));
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset node_size_logn = 1;
    /* 定位到最底层叶子节点，并向上找到为0的节点(被分配的节点) */
    LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id index = offset + LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, 0)-1);
    for (; LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, index); index = LIBYUC_SPACE_MANAGER_BUDDY_PARENT(index)) {
        node_size_logn++;
        if (index == 1) return -1; /* 没有找到被分配的节点 */
    }
    return LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(node_size_logn-1);
}
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset BuddyGetMaxFreeCount(Buddy* buddy) {
    return LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, 1)-1);
}
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset BuddyGetMaxCount(Buddy* buddy) {
    return LIBYUC_ALGORITHM_TWO_TO_POWER_OF_2(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get(buddy, buddy->logn, 0)-1);
}

#endif

#undef Buddy

#undef BuddyCreate
#undef BuddyInit
#undef BuddyAlloc
#undef BuddyAllocByOffset
#undef BuddyFree
#undef BuddyGetAllocBlockSize
#undef BuddyGetMaxFreeCount
#undef BuddyGetMaxCount


#undef LIBYUC_SPACE_MANAGER_BUDDY_CLASS_NAME
#undef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Const_ElementCount
#undef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Const_InvalidId
#undef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Const_InvalidDynamicArray
#undef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id
#undef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset
#undef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Set
#undef LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Get
#undef LIBYUC_SPACE_MANAGER_BUDDY_ALLOCATOR_Create
#undef LIBYUC_SPACE_MANAGER_BUDDY_ALLOCATOR_CreateMultiple
#undef LIBYUC_SPACE_MANAGER_BUDDY_ALLOCATOR_Release


#undef LIBYUC_SPACE_MANAGER_BUDDY_MODE_STATIC
#undef LIBYUC_SPACE_MANAGER_BUDDY_MODE_DYNAMIC

#ifdef __cplusplus
}
#endif
