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

#include <libyuc/space_manager/buddy.def>

typedef struct Buddy {
    /*uint8_t size;*/
    uint8_t logn[];        /* 实际存放指数+1，因为0用来表示没有空间，而2^0应该是1 */
} Buddy;


Buddy* BuddyCreate(LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset size);
bool BuddyInit(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset size);
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id BuddyAlloc(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset size);
bool BuddyAllocByOffset(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id offset, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset size);
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset BuddyFree(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id offset);
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset BuddyGetAllocBlockSize(Buddy* buddy, LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Id offset);
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset BuddyGetMaxFreeCount(Buddy* buddy);
LIBYUC_SPACE_MANAGER_BUDDY_INDEXER_Type_Offset BuddyGetMaxCount(Buddy* buddy);

#include <libyuc/space_manager/buddy.undef>

#ifdef __cplusplus
}
#endif
