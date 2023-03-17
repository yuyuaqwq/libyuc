/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_SPACE_MANAGER_BTM_H_
#define CUTILS_CONTAINER_SPACE_MANAGER_BTM_H_

/*
* 空间管理 - 边界标识法(boundary tag method)
*/
#define CUTILS_CONTAINER_SPACE_MANAGER_BTM_DECLARATION(btm_type_name, id_type, referencer) \
    typedef struct _##btm_type_name##BtmBlockHead { \
        id_type next_tag; \
        id_type size; \
    } btm_type_name##BtmBlockHead; \
    typedef struct _##btm_type_name##BtmBlockFoot { \
        id_type head_tag; \
    } btm_type_name##BtmBlockFoot; \
    typedef struct _##btm_type_name##BtmManager { \
        id_type first; \
        /* uint8_t space[]; */ \
    } btm_type_name##BtmManager; \

#define CUTILS_CONTAINER_SPACE_MANAGER_BTM_GetTag(id) (id & 1)
#define CUTILS_CONTAINER_SPACE_MANAGER_BTM_GetId(id) (id & (~1))
#define CUTILS_CONTAINER_SPACE_MANAGER_BTM_SetTag(old_id, tag) (old_id = CUTILS_CONTAINER_SPACE_MANAGER_BTM_GetId(id) | tag)
#define CUTILS_CONTAINER_SPACE_MANAGER_BTM_SetId(old_id, id) (old_id = id | CUTILS_CONTAINER_SPACE_MANAGER_BTM_GetTag(old_id))
#define CUTILS_CONTAINER_BOUNDARY_TAG_METHOD_DEFINE(btm_type_name, id_type, referencer) \
    void btm_type_name##BtmInit(btm_type_name##BtmManager* btm, id_type space_size) { \
        btm->first = sizeof(btm_type_name##BtmManager); \
        btm_type_name##BtmBlockHead* head = (btm_type_name##BtmBlockHead*)((uintptr_t)btm + sizeof(btm_type_name##BtmManager)); \
        head->next_tag = referencer##_InvalidId; \
        head->size = space_size - sizeof(btm_type_name##BtmManager) - sizeof(btm_type_name##BtmBlockHead); \
    } \
    type_id btm_type_name##BtmAlloc(btm_type_name##BtmManager* btm, type_id size) { \
        size += sizeof(btm_type_name##BtmBlockFoot); \
        id_type cur_block = btm->first; \
        while (cur_block != referencer##_InvalidId) { \
            btm_type_name##BtmBlockHead* head = (btm_type_name##BtmBlockHead*)((uintptr_t)btm + cur_block); \
            if (head->size > size) { \
                id_type split_block = cur_block + sizeof(btm_type_name##BtmBlockHead) + size + sizeof(btm_type_name##BtmBlockFoot); \
                btm_type_name##BtmBlockHead* split_head = (btm_type_name##BtmBlockHead*)((uintptr_t)btm + split_block); \
                split_head->size = ; \
                return cur_block + sizeof(btm_type_name##BtmBlockHead); \
            } \
        } \
        cur = CUTILS_CONTAINER_BOUNDARY_TAG_METHOD_GetId(head->next_tag); \
    } \


#endif // CUTILS_CONTAINER_SPACE_MANAGER_BTM_H_