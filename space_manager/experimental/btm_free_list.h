/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef CUTILS_CONTAINER_SPACE_MANAGER_BTM_FREE_LIST_H_
#define CUTILS_CONTAINER_SPACE_MANAGER_BTM_FREE_LIST_H_

#include <CUtils/space_manager/free_list.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 边界标识法(boundary tag method)空闲链表
*/
#define CUTILS_CONTAINER_SPACE_MANAGER_BTM_FREE_LIST_DECLARATION(btm_type_name, id_type, element_type, list_count, obj_coun) \
    CUTILS_CONTAINER_SPACE_MANAGER_FREE_LIST_DECLARATION(btm_type_name##BtmBase, id_type, ) \
    typedef struct _##btm_type_name##BtmBlockFoot { \
        id_type head_tag; \
    } btm_type_name##BtmBlockFoot; \
    typedef struct _##btm_type_name##BtmFreeList { \
        btm_type_name##BtmBaseFreeList base_free_list; \
    } btm_type_name##BtmFreeList; \


#define CUTILS_CONTAINER_SPACE_MANAGER_BTM_FREE_LIST_DEFINE(btm_type_name, id_type, referencer, accessor) \
    static btm_type_name##BtmBlockFoot* btm_type_name##BtmFreeListGetEntryFoot(id_type end) { \
        return (btm_type_name##BtmBlockFoot*)(end - sizeof(btm_type_name##BtmBlockFoot)); \
    } \
    void btm_type_name##BtmFreeListInit(btm_type_name##BtmFreeList* free_list, id_type element_count) { \
        btm_type_name##BtmBaseFreeListInit(&free_list->base_free_list, element_count); \
        free_list_type_name##FreeBlockEntry* block = (free_list_type_name##FreeBlockEntry*)(&head->obj_arr[0]); \
        block->next_block_offset = referencer##_InvalidId; \
        block->count = element_count; \
    } \
    id_type btm_type_name##BtmFreeListAlloc(btm_type_name##BtmFreeList* free_list, id_type list_order, id_type count) { \
        count += sizeof(btm_type_name##BtmBlockFoot); \
        id_type base = btm_type_name##BtmBaseFreeListAlloc(&free_list->base_free_list, list_order, count); \
        btm_type_name##BtmBlockFoot* foot = btm_type_name##BtmFreeListGetEntryFoot(base + count); \
        accessor##_SetId(foot, base); \
        accessor##_SetTag(foot, true); \
    } \
    void btm_type_name##BtmFreeListFree(btm_type_name##BtmFreeList* free_list, id_type base, id_type count) { \
         \
    } \

#define CUTILS_CONTAINER_SPACE_MANAGER_BTM_FREE_LIST_ACCESSOR_GetTag(foot) ((foot)->head_tag & 1)
#define CUTILS_CONTAINER_SPACE_MANAGER_BTM_FREE_LIST_ACCESSOR_GetId(foot) ((foot)->head_tag & (~1))
#define CUTILS_CONTAINER_SPACE_MANAGER_BTM_FREE_LIST_ACCESSOR_SetTag(foot, tag) ((foot)->head_tag = CUTILS_CONTAINER_SPACE_MANAGER_BTM_FREE_LIST_ACCESSOR_GetId(foot) | tag)
#define CUTILS_CONTAINER_SPACE_MANAGER_BTM_FREE_LIST_ACCESSOR_SetId(foot, id) ((foot)->head_tag = id | CUTILS_CONTAINER_SPACE_MANAGER_BTM_FREE_LIST_ACCESSOR_GetTag(foot))
#define CUTILS_CONTAINER_SPACE_MANAGER_BTM_FREE_LIST_ACCESSOR CUTILS_CONTAINER_SPACE_MANAGER_BTM_FREE_LIST_ACCESSOR

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_SPACE_MANAGER_BTM_H_