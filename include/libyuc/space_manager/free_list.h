/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#include <libyuc/basic.h>

/*
* 常规空闲链表
*/
#include <libyuc/space_manager/free_list.def>

typedef struct FreeBlockEntry {
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id next_block_offset;
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id count;
} FreeBlockEntry;

typedef struct FreeList {
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id first_block[LIBYUC_SPACE_MANAGER_FREE_LIST_Const_StaticListCount];        /* 分别指向不同队列的第一个块 */
    LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Element obj_arr[LIBYUC_SPACE_MANAGER_FREE_LIST_Const_StaticElementCount];
} FreeList;

void FreeListInit(FreeList* head, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id element_count);
LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id FreeListAlloc(FreeList* head, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id list_order, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id* count_);
void FreeListFree(FreeList* head, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id list_order, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id free_offset, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id* count_);
LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id FreeListGetFreeBlockSize(FreeList* head, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id list_order);
LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id FreeListGetMaxFreeBlockSize(FreeList* head, LIBYUC_SPACE_MANAGER_FREE_LIST_INDEXER_Type_Id list_order);


#include <libyuc/space_manager/free_list.undef>

