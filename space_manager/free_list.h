/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME
#define LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME
#endif

#ifndef LIBYUC_SPACE_MANAGER_FREE_LIST_Const_StaticElementCount
#define LIBYUC_SPACE_MANAGER_FREE_LIST_Const_StaticElementCount 0
#endif

#ifndef LIBYUC_SPACE_MANAGER_FREE_LIST_Const_StaticListCount
#define LIBYUC_SPACE_MANAGER_FREE_LIST_Const_StaticListCount 1
#endif

#ifndef LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_DynamicArray
#define LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_DynamicArray LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Element*
#endif

#ifndef  LIBYUC_SPACE_MANAGER_FREE_LIST_ACCESSOR_GetNext
#define LIBYUC_SPACE_MANAGER_FREE_LIST_ACCESSOR_GetNext(main_obj, obj) ((obj)->next)
#endif

#ifndef  LIBYUC_SPACE_MANAGER_FREE_LIST_ACCESSOR_SetNext
#define LIBYUC_SPACE_MANAGER_FREE_LIST_ACCESSOR_SetNext(main_obj, obj, new_next) ((obj)->next = (new_next))
#endif

#ifndef LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Element
#define LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Element FreeBlockEntry
#endif

#ifndef LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id
#define LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id size_t
#endif

#ifndef LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Offset
#define LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Offset LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id
#endif

#ifndef LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Const_InvalidId
#define LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Const_InvalidId -1
#endif


/*
* 常规空闲链表
*/

#define FreeBlockEntry MAKE_NAME(LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME, FreeBlockEntry)
#define FreeList MAKE_NAME(LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME, FreeList)

typedef struct FreeBlockEntry {
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id next_block_offset;
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id count;
} FreeBlockEntry;

typedef struct FreeList {
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id first_block[LIBYUC_SPACE_MANAGER_FREE_LIST_Const_StaticListCount];        /* 分别指向不同队列的第一个块 */
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Element obj_arr[LIBYUC_SPACE_MANAGER_FREE_LIST_Const_StaticElementCount];
} FreeList;

#define FreeListInit MAKE_NAME(LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME, FreeListInit)
#define FreeListAlloc MAKE_NAME(LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME, FreeListAlloc)
#define FreeListFree MAKE_NAME(LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME, FreeListFree)
#define FreeListGetFreeBlockSize MAKE_NAME(LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME, FreeListGetFreeBlockSize)
#define FreeListGetMaxFreeBlockSize MAKE_NAME(LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME, FreeListGetMaxFreeBlockSize)

LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id FreeListAlloc(FreeList* head, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id list_order, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id* count_);
void FreeListFree(FreeList* head, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id list_order, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id free_offset, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id* count_);
LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id FreeListGetFreeBlockSize(FreeList* head, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id list_order);
LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id FreeListGetMaxFreeBlockSize(FreeList* head, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id list_order);


#ifdef LIBYUC_TEMPLATE_C_INCLUDE
/*
* 初始化
*/
void FreeListInit(FreeList* head, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id element_count) {
    head->first_block[0] = 0;
    for (int16_t i = 1; i < LIBYUC_SPACE_MANAGER_FREE_LIST_Const_StaticListCount; i++) {
        head->first_block[i] = LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Const_InvalidId;
    }
    FreeBlockEntry* block = (FreeBlockEntry*)(&head->obj_arr[0]);
    block->next_block_offset = LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Const_InvalidId;
    block->count = element_count - element_count % sizeof(FreeBlockEntry);
}
/*
* 分配块，返回偏移
*/
LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id FreeListAlloc(FreeList* head, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id list_order, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id* count_) {
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id count = *count_;
    if (count % sizeof(FreeBlockEntry)) {
        count = count + ((count % sizeof(FreeBlockEntry) ? sizeof(FreeBlockEntry) - count % sizeof(FreeBlockEntry) : 0));
        *count_ = count;
    }
    FreeBlockEntry* prev_block = (FreeBlockEntry*)(&head->first_block[list_order]);
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id free_offset = head->first_block[list_order];
    while (free_offset != LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Const_InvalidId) {
        FreeBlockEntry* block = (FreeBlockEntry*)(&head->obj_arr[free_offset]);
        if (block->count > count) {
            FreeBlockEntry* new_block = (FreeBlockEntry*)(&head->obj_arr[free_offset+ count]);
            new_block->next_block_offset = block->next_block_offset;
            new_block->count = block->count - count;
            prev_block->next_block_offset += count;
            return free_offset;
        }
        else if (block->count == count) {
            prev_block->next_block_offset = block->next_block_offset;
            return free_offset;
        }
        free_offset = block->next_block_offset;
        prev_block = block;
    };
    return LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Const_InvalidId;
}
/*
* 释放块
*/
void FreeListFree(FreeList* head, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id list_order, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id free_offset, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id* count_) {
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id count = *count_;
    if (count % sizeof(FreeBlockEntry)) {
        count = count + ((count % sizeof(FreeBlockEntry) ? sizeof(FreeBlockEntry) - count % sizeof(FreeBlockEntry) : 0));
        *count_ = count;
    }
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id cur_offset = head->first_block[list_order];
    FreeBlockEntry* prev_block = (FreeBlockEntry*)(&head->first_block[list_order]);
    FreeBlockEntry* cur_block;
    /* 尝试合并与当前块连续的前后空闲块 */
    FreeBlockEntry* free_prev_prev_block = NULL, * free_next_prev_block = NULL;
    FreeBlockEntry* free_prev_block = NULL, * free_next_block = NULL;
    while (cur_offset != LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Const_InvalidId) {
        cur_block = (FreeBlockEntry*)(&head->obj_arr[cur_offset]);
        if (!free_next_block && free_offset + count == cur_offset) {
            /* 找到连续的空闲后继块 */
            if (free_prev_block) {
                /* 前继块需要删除 */
                free_prev_prev_block->next_block_offset = free_prev_block->next_block_offset;
            }
            count += cur_block->count;
            LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id next_offset = cur_block->next_block_offset;
            cur_block = (FreeBlockEntry*)(&head->obj_arr[free_offset]);
            cur_block->count = count;
            cur_block->next_block_offset = next_offset;
            prev_block->next_block_offset = free_offset;
            free_next_prev_block = prev_block;
            free_next_block = cur_block;
        }
        else if (!free_prev_block && cur_offset + cur_block->count == free_offset) {
            /* 找到连续的空闲前继块，更新count和free_offset是为了找可能存在的连续的空闲下一块 */
            if (free_next_block) {
                /* 后继块需要删除 */
                free_next_prev_block->next_block_offset = free_next_block->next_block_offset;
            }
            free_offset = cur_offset;
            count += cur_block->count;
            cur_block->count = count;
            free_prev_prev_block = prev_block;
            free_prev_block = cur_block;
        }
        else {
            /* 没有合并才更新，找到连续的空闲下一块也不更新这块逻辑比较复杂
                主要为了使得下一次循环若找到连续的空闲上一块时，prev能够正确的指向合并后的free_block */
            prev_block = cur_block;
        }
        if (free_prev_block && free_next_block) break;
        cur_offset = cur_block->next_block_offset;
    }
    if (!free_prev_block && !free_next_block) {
        cur_block = (FreeBlockEntry*)(&head->obj_arr[free_offset]);
        cur_block->next_block_offset = head->first_block[list_order];
        cur_block->count = count;
        head->first_block[list_order] = free_offset;
    }
}
/*
* 查询最大块长度
*/
LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id FreeListGetMaxFreeBlockSize(FreeList* head, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id list_order) {
    FreeBlockEntry* prev_block = (FreeBlockEntry*)((uintptr_t)&head->first_block[list_order]);
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id free_offset = head->first_block[list_order];
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id max = 0;
    while (free_offset != LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Const_InvalidId) {
        FreeBlockEntry* block = (FreeBlockEntry*)(&head->obj_arr[free_offset]);
        if (block->count > max) {
            max = block->count;
        }
        free_offset = block->next_block_offset;
    }
    return max;
}
LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id FreeListGetFreeBlockSize(FreeList* head, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id list_order) {
    FreeBlockEntry* prev_block = (FreeBlockEntry*)((uintptr_t)&head->first_block[list_order]);
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id free_offset = head->first_block[list_order];
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id max = 0;
    while (free_offset != LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Const_InvalidId) {
        FreeBlockEntry* block = (FreeBlockEntry*)(&head->obj_arr[free_offset]);
        if (block->count > max) {
            max += block->count;
        }
        free_offset = block->next_block_offset;
    }
    return max;
}

#endif

#define FreeBlockEntry MAKE_NAME(LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME, FreeBlockEntry)
#define FreeList MAKE_NAME(LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME, FreeList)


#define FreeListInit MAKE_NAME(LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME, FreeListInit)
#define FreeListAlloc MAKE_NAME(LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME, FreeListAlloc)
#define FreeListFree MAKE_NAME(LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME, FreeListFree)
#define FreeListGetFreeBlockSize MAKE_NAME(LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME, FreeListGetFreeBlockSize)
#define FreeListGetMaxFreeBlockSize MAKE_NAME(LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME, FreeListGetMaxFreeBlockSize)


#undef FreeBlockEntry
#undef FreeList


#undef FreeListInit
#undef FreeListAlloc
#undef FreeListFree
#undef FreeListGetFreeBlockSize
#undef FreeListGetMaxFreeBlockSize

#undef LIBYUC_SPACE_MANAGER_FREE_LIST_CLASS_NAME
#undef LIBYUC_SPACE_MANAGER_FREE_LIST_Const_StaticListCount
#undef LIBYUC_SPACE_MANAGER_FREE_LIST_Const_StaticElementCount
#undef LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Const_InvalidId
#undef LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Element
#undef LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id
#undef LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Offset
#undef LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_DynamicArray

#undef LIBYUC_SPACE_MANAGER_FREE_LIST_MODE_STATIC
#undef LIBYUC_SPACE_MANAGER_FREE_LIST_MODE_DYNAMIC

#ifdef __cplusplus
}
#endif
