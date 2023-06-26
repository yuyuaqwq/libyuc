/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef CUTILS_CONTAINER_SPACE_MANAGER_FREE_LIST_H_
#define CUTILS_CONTAINER_SPACE_MANAGER_FREE_LIST_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 常规空闲链表
*/
#define CUTILS_CONTAINER_SPACE_MANAGER_FREE_LIST_DECLARATION_1(free_list_type_name, id_type) \
    typedef struct _##free_list_type_name##FreeBlockEntry { \
        id_type next_block_offset; \
        id_type count; \
    } free_list_type_name##FreeBlockEntry; \


#define CUTILS_CONTAINER_SPACE_MANAGER_FREE_LIST_DECLARATION_2(free_list_type_name, id_type, element_type, list_count, obj_count) \
    typedef struct _##free_list_type_name##FreeList { \
        id_type first_block[list_count];        /* 分别指向不同队列的第一个块 */ \
        element_type obj_arr[obj_count]; \
    } free_list_type_name##FreeList; \
    id_type free_list_type_name##FreeListAlloc(free_list_type_name##FreeList* head, id_type list_order, id_type* count_); \
    id_type free_list_type_name##FreeListAlloc(free_list_type_name##FreeList* head, id_type list_order, id_type* count_); \
    void free_list_type_name##FreeListFree(free_list_type_name##FreeList* head, id_type list_order, id_type free_offset, id_type* count_); \
    id_type free_list_type_name##FreeListGetFreeBlockSize(free_list_type_name##FreeList* head, id_type list_order); \
    id_type free_list_type_name##FreeListGetMaxFreeBlockSize(free_list_type_name##FreeList* head, id_type list_order); \

#define CUTILS_CONTAINER_SPACE_MANAGER_FREE_LIST_DECLARATION(free_list_type_name, id_type, element_type, list_count, obj_count) \
    CUTILS_CONTAINER_SPACE_MANAGER_FREE_LIST_DECLARATION_1(free_list_type_name, id_type) \
    CUTILS_CONTAINER_SPACE_MANAGER_FREE_LIST_DECLARATION_2(free_list_type_name, id_type, element_type, list_count, obj_count)


#define CUTILS_CONTAINER_SPACE_MANAGER_FREE_LIST_DEFINE(free_list_type_name, id_type, element_type, referencer, list_count) \
    /*
    * 初始化
    */ \
    void free_list_type_name##FreeListInit(free_list_type_name##FreeList* head, id_type element_count) { \
        head->first_block[0] = 0; \
        for (int16_t i = 1; i < list_count; i++) { \
            head->first_block[i] = referencer##_InvalidId; \
        } \
        free_list_type_name##FreeBlockEntry* block = (free_list_type_name##FreeBlockEntry*)(&head->obj_arr[0]); \
        block->next_block_offset = referencer##_InvalidId; \
        block->count = element_count - element_count % sizeof(free_list_type_name##FreeBlockEntry); \
    } \
    /*
    * 分配块，返回偏移
    */ \
    id_type free_list_type_name##FreeListAlloc(free_list_type_name##FreeList* head, id_type list_order, id_type* count_) { \
        id_type count = *count_; \
        if (count % sizeof(free_list_type_name##FreeBlockEntry)) { \
		    count = count + ((count % sizeof(free_list_type_name##FreeBlockEntry) ? sizeof(free_list_type_name##FreeBlockEntry) - count % sizeof(free_list_type_name##FreeBlockEntry) : 0)); \
            *count_ = count; \
        } \
        free_list_type_name##FreeBlockEntry* prev_block = (free_list_type_name##FreeBlockEntry*)(&head->first_block[list_order]); \
        id_type free_offset = head->first_block[list_order]; \
        while (free_offset != referencer##_InvalidId) { \
            free_list_type_name##FreeBlockEntry* block = (free_list_type_name##FreeBlockEntry*)(&head->obj_arr[free_offset]); \
            if (block->count > count) { \
                free_list_type_name##FreeBlockEntry* new_block = (free_list_type_name##FreeBlockEntry*)(&head->obj_arr[free_offset+ count]); \
                new_block->next_block_offset = block->next_block_offset; \
                new_block->count = block->count - count; \
                prev_block->next_block_offset += count; \
                return free_offset; \
            } \
            else if (block->count == count) { \
                prev_block->next_block_offset = block->next_block_offset; \
                return free_offset; \
            } \
            free_offset = block->next_block_offset; \
            prev_block = block; \
        }; \
        return referencer##_InvalidId; \
    } \
    /*
    * 释放块
    */ \
    void free_list_type_name##FreeListFree(free_list_type_name##FreeList* head, id_type list_order, id_type free_offset, id_type* count_) { \
        id_type count = *count_; \
        if (count % sizeof(free_list_type_name##FreeBlockEntry)) { \
            count = count + ((count % sizeof(free_list_type_name##FreeBlockEntry) ? sizeof(free_list_type_name##FreeBlockEntry) - count % sizeof(free_list_type_name##FreeBlockEntry) : 0)); \
            *count_ = count; \
        } \
        id_type cur_offset = head->first_block[list_order]; \
        free_list_type_name##FreeBlockEntry* prev_block = (free_list_type_name##FreeBlockEntry*)(&head->first_block[list_order]); \
        free_list_type_name##FreeBlockEntry* cur_block; \
        /* 尝试合并与当前块连续的前后空闲块 */ \
        free_list_type_name##FreeBlockEntry* free_prev_prev_block = NULL, * free_next_prev_block = NULL; \
        free_list_type_name##FreeBlockEntry* free_prev_block = NULL, * free_next_block = NULL; \
        while (cur_offset != referencer##_InvalidId) { \
            cur_block = (free_list_type_name##FreeBlockEntry*)(&head->obj_arr[cur_offset]); \
            if (!free_next_block && free_offset + count == cur_offset) { \
                /* 找到连续的空闲后继块 */ \
                if (free_prev_block) { \
                    /* 前继块需要删除 */ \
                    free_prev_prev_block->next_block_offset = free_prev_block->next_block_offset; \
                } \
                count += cur_block->count; \
                id_type next_offset = cur_block->next_block_offset; \
                cur_block = (free_list_type_name##FreeBlockEntry*)(&head->obj_arr[free_offset]); \
                cur_block->count = count; \
                cur_block->next_block_offset = next_offset; \
                prev_block->next_block_offset = free_offset; \
                free_next_prev_block = prev_block; \
                free_next_block = cur_block; \
            } \
            else if (!free_prev_block && cur_offset + cur_block->count == free_offset) { \
                /* 找到连续的空闲前继块，更新count和free_offset是为了找可能存在的连续的空闲下一块 */ \
                if (free_next_block) { \
                    /* 后继块需要删除 */ \
                    free_next_prev_block->next_block_offset = free_next_block->next_block_offset; \
                } \
                free_offset = cur_offset; \
                count += cur_block->count; \
                cur_block->count = count; \
                free_prev_prev_block = prev_block; \
                free_prev_block = cur_block; \
            } \
            else { \
                /* 没有合并才更新，找到连续的空闲下一块也不更新这块逻辑比较复杂
                 主要为了使得下一次循环若找到连续的空闲上一块时，prev能够正确的指向合并后的free_block */ \
                prev_block = cur_block; \
            } \
            if (free_prev_block && free_next_block) break; \
            cur_offset = cur_block->next_block_offset; \
        } \
        if (!free_prev_block && !free_next_block) { \
            cur_block = (free_list_type_name##FreeBlockEntry*)(&head->obj_arr[free_offset]); \
            cur_block->next_block_offset = head->first_block[list_order]; \
            cur_block->count = count; \
            head->first_block[list_order] = free_offset; \
        } \
    } \
    /*
    * 查询最大块长度
    */ \
    id_type free_list_type_name##FreeListGetMaxFreeBlockSize(free_list_type_name##FreeList* head, id_type list_order) { \
        free_list_type_name##FreeBlockEntry* prev_block = (free_list_type_name##FreeBlockEntry*)((uintptr_t)&head->first_block[list_order]); \
        id_type free_offset = head->first_block[list_order]; \
        id_type max = 0; \
        while (free_offset != referencer##_InvalidId) { \
            free_list_type_name##FreeBlockEntry* block = (free_list_type_name##FreeBlockEntry*)(&head->obj_arr[free_offset]); \
            if (block->count > max) { \
                max = block->count; \
            } \
            free_offset = block->next_block_offset; \
        } \
        return max; \
    } \
    id_type free_list_type_name##FreeListGetFreeBlockSize(free_list_type_name##FreeList* head, id_type list_order) { \
        free_list_type_name##FreeBlockEntry* prev_block = (free_list_type_name##FreeBlockEntry*)((uintptr_t)&head->first_block[list_order]); \
        id_type free_offset = head->first_block[list_order]; \
        id_type max = 0; \
        while (free_offset != referencer##_InvalidId) { \
            free_list_type_name##FreeBlockEntry* block = (free_list_type_name##FreeBlockEntry*)(&head->obj_arr[free_offset]); \
            if (block->count > max) { \
                max += block->count; \
            } \
            free_offset = block->next_block_offset; \
        } \
        return max; \
    } \

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_SPACE_MANAGER_FREE_LIST_H_