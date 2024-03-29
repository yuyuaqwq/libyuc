#include <libyuc/space_manager/buddy.def>


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


#include <libyuc/space_manager/buddy.undef>
