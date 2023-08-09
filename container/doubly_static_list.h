/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_H_
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_H_

#include <libyuc/basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_DECLARATION_1(ds_list_type_name, id_offset_type) \
    /*
    * 若只有一条队列，由Pop分配的节点，此结构体可以被覆盖，可以使用union
    */ \
    typedef struct _##ds_list_type_name##DoublyStaticListEntry{ \
        id_offset_type prev; \
        id_offset_type next; \
    } ds_list_type_name##DoublyStaticListEntry; \

#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_DECLARATION_2(ds_list_type_name, id_offset_type, element_type, list_count, obj_arr_count) \
    typedef struct _##ds_list_type_name##DoublyStaticList{ \
        id_offset_type list_first[list_count]; /* 最少应为1 */ \
        element_type obj_arr[obj_arr_count]; \
    } ds_list_type_name##DoublyStaticList; \
    \
    void ds_list_type_name##DoublyStaticListInit(ds_list_type_name##DoublyStaticList* list, id_offset_type count); \
    id_offset_type ds_list_type_name##DoublyStaticListPop(ds_list_type_name##DoublyStaticList* list, id_offset_type list_order); \
    void ds_list_type_name##DoublyStaticListPush(ds_list_type_name##DoublyStaticList* list, id_offset_type list_order, id_offset_type index); \
    void ds_list_type_name##DoublyStaticListSwitch(ds_list_type_name##DoublyStaticList* list, id_offset_type list_order, id_offset_type index, id_offset_type new_list_order); \

#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_DECLARATION(ds_list_type_name, id_offset_type, element_type, list_count, obj_arr_count) \
    LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_DECLARATION_1(ds_list_type_name, id_offset_type) \
    LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_DECLARATION_2(ds_list_type_name, id_offset_type, element_type, list_count, obj_arr_count) \


/*
* 访问器需要提供_GetNext、_SetNext、_GetPrev、_SetPrev方法
*/
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_DEFINE(ds_list_type_name, id_offset_type, element_type, referencer, accessor, list_count) \
    void ds_list_type_name##DoublyStaticListInit(ds_list_type_name##DoublyStaticList* list, id_offset_type count) { \
        list->list_first[0] = 0; \
        id_offset_type i = 0; \
        for (; i < count - 1; i++) { \
            accessor##_SetNext(list, &list->obj_arr[i], i + 1); \
            if (i == 0) { \
                accessor##_SetPrev(list, &list->obj_arr[i], referencer##_InvalidId); \
            } \
            else { \
                accessor##_SetPrev(list, &list->obj_arr[i], i - 1); \
            } \
        } \
        accessor##_SetNext(list, &list->obj_arr[i], referencer##_InvalidId); \
        for (i = 1; i < list_count; i++) { \
            list->list_first[i] = referencer##_InvalidId; \
        } \
    } \
    id_offset_type ds_list_type_name##DoublyStaticListDelete(ds_list_type_name##DoublyStaticList* list, id_offset_type list_order, id_offset_type delete_index) { \
        if (accessor##_GetPrev(list, &list->obj_arr[delete_index]) == referencer##_InvalidId) { \
            list->list_first[list_order] = accessor##_GetNext(list, &list->obj_arr[delete_index]); \
        } \
        else { \
            accessor##_SetNext(list, &list->obj_arr[accessor##_GetPrev(list, &list->obj_arr[delete_index])], accessor##_GetNext(list, &list->obj_arr[delete_index])); \
        } \
        \
        if (accessor##_GetNext(list, &list->obj_arr[delete_index]) != referencer##_InvalidId) { \
            accessor##_SetPrev(list, &list->obj_arr[accessor##_GetNext(list, &list->obj_arr[delete_index])], accessor##_GetPrev(list, &list->obj_arr[delete_index])); \
        } \
        return delete_index; \
    } \
    id_offset_type ds_list_type_name##DoublyStaticListPop(ds_list_type_name##DoublyStaticList* list, id_offset_type list_order) { \
        if (list->list_first[list_order] == referencer##_InvalidId) { \
            return referencer##_InvalidId; \
        } \
        return ds_list_type_name##DoublyStaticListDelete(list, list_order, list->list_first[list_order]); \
    } \
    void ds_list_type_name##DoublyStaticListInsert(ds_list_type_name##DoublyStaticList* list, id_offset_type list_order, id_offset_type prev_index, id_offset_type insert_index) { \
        if (prev_index == referencer##_InvalidId) { \
            accessor##_SetPrev(list, &list->obj_arr[insert_index], referencer##_InvalidId); \
            accessor##_SetNext(list, &list->obj_arr[insert_index], list->list_first[list_order]); \
            list->list_first[list_order] = insert_index; \
        } \
        else { \
            accessor##_SetPrev(list, &list->obj_arr[insert_index], prev_index); \
            accessor##_SetNext(list, &list->obj_arr[insert_index], accessor##_GetNext(list, &list->obj_arr[prev_index])); \
            accessor##_SetNext(list, &list->obj_arr[prev_index], insert_index); \
        } \
        if (accessor##_GetNext(list, &list->obj_arr[insert_index]) != referencer##_InvalidId) { \
            accessor##_SetPrev(list, &list->obj_arr[accessor##_GetNext(list, &list->obj_arr[insert_index])], insert_index); \
        } \
    } \
    void ds_list_type_name##DoublyStaticListPush(ds_list_type_name##DoublyStaticList* list, id_offset_type list_order, id_offset_type push_index) { \
         ds_list_type_name##DoublyStaticListInsert(list, list_order, referencer##_InvalidId, push_index); \
    } \
    void ds_list_type_name##DoublyStaticListSwitch(ds_list_type_name##DoublyStaticList* list, id_offset_type list_order, id_offset_type index, id_offset_type new_list_order) { \
        ds_list_type_name##DoublyStaticListDelete(list, list_order, index); \
        ds_list_type_name##DoublyStaticListPush(list, new_list_order, index); \
    } \

#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_DEFAULT_REFERENCER_Dereference(LIST, ENTRY)
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_DEFAULT_REFERENCER_Reference(LIST, ENTRY) (ENTRY)
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_DEFAULT_REFERENCER_InvalidId (-1)
#define LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_DEFAULT_REFERENCER LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_DEFAULT_REFERENCER


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_DOUBLY_STATIC_LIST_H_