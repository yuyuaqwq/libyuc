/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_STATIC_LIST_H_
#define LIBYUC_CONTAINER_STATIC_LIST_H_

#include <libyuc/basic.h>
#include <libyuc/container/vector.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIBYUC_CONTAINER_STATIC_LIST_DECLARATION_1(static_list_type_name, id_offset_type) \
    typedef struct _##static_list_type_name##StaticListEntry { \
        id_offset_type next; \
    } static_list_type_name##StaticListEntry; \

#define LIBYUC_CONTAINER_STATIC_LIST_DECLARATION_2(static_list_type_name, id_offset_type, element_type, list_count, obj_arr_count) \
    typedef struct _##static_list_type_name##StaticList { \
        id_offset_type list_first[list_count]; /* 最少应为1 */ \
        element_type obj_arr[obj_arr_count]; \
    } static_list_type_name##StaticList; \
    \
    void static_list_type_name##StaticListInit(static_list_type_name##StaticList* list, id_offset_type count); \
    id_offset_type static_list_type_name##StaticListPop(static_list_type_name##StaticList* list, id_offset_type list_order); \
    void static_list_type_name##StaticListPush(static_list_type_name##StaticList* list, id_offset_type list_order, id_offset_type index); \
    id_offset_type static_list_type_name##StaticListDelete(static_list_type_name##StaticList* list, id_offset_type list_order, id_offset_type prev_id, id_offset_type delete_id); \
    void static_list_type_name##StaticListSwitch(static_list_type_name##StaticList* list, id_offset_type list_order, id_offset_type prev_id, id_offset_type id, id_offset_type new_list_order); \
    id_offset_type static_list_type_name##StaticListIteratorFirst(static_list_type_name##StaticList* list, id_offset_type list_order); \
    id_offset_type static_list_type_name##StaticListIteratorNext(static_list_type_name##StaticList* list, id_offset_type cur_id); \

#define LIBYUC_CONTAINER_STATIC_LIST_DECLARATION(static_list_type_name, id_offset_type, element_type, list_count, obj_arr_count) \
    LIBYUC_CONTAINER_STATIC_LIST_DECLARATION_1(static_list_type_name, id_offset_type) \
    LIBYUC_CONTAINER_STATIC_LIST_DECLARATION_2(static_list_type_name, id_offset_type, element_type, list_count, obj_arr_count) \

/*
* 访问器需要提供_GetNext、_SetNext方法
*/
#define LIBYUC_CONTAINER_STATIC_LIST_DEFINE(static_list_type_name, id_offset_type, element_type, accessor, referencer, list_count) \
    void static_list_type_name##StaticListInit(static_list_type_name##StaticList* list, id_offset_type count) { \
        list->list_first[0] = 0; \
        id_offset_type i = 0; \
        for (; i < count - 1; i++) { \
            accessor##_SetNext(list, &list->obj_arr[i], i + 1); \
        } \
        accessor##_SetNext(list, &list->obj_arr[i], referencer##_InvalidId); \
        \
        for (i = 1; i < list_count; i++) { \
            list->list_first[i] = referencer##_InvalidId; \
        } \
    } \
    void static_list_type_name##StaticListExpand(static_list_type_name##StaticList* list, id_offset_type old_count, id_offset_type new_count) { \
        id_offset_type old_first = list->list_first[0]; \
        list->list_first[0] = new_count - 1; \
        id_offset_type i = old_count; \
        for (; i < new_count - 1; i++) { \
            accessor##_SetNext(list, &list->obj_arr[i], i + 1); \
        } \
        accessor##_SetNext(list, &list->obj_arr[i], old_first); \
    } \
    id_offset_type static_list_type_name##StaticListPop(static_list_type_name##StaticList* list, id_offset_type list_order) { \
        if (list->list_first[list_order] == referencer##_InvalidId) { \
            return referencer##_InvalidId; \
        } \
        id_offset_type index = list->list_first[list_order]; \
        list->list_first[list_order] = accessor##_GetNext(list, &list->obj_arr[index]); \
        return index; \
    } \
    void static_list_type_name##StaticListPush(static_list_type_name##StaticList* list, id_offset_type list_order, id_offset_type index) { \
        accessor##_SetNext(list, &list->obj_arr[index], list->list_first[list_order]); \
        list->list_first[list_order] = index; \
    } \
    id_offset_type static_list_type_name##StaticListDelete(static_list_type_name##StaticList* list, id_offset_type list_order, id_offset_type prev_id, id_offset_type delete_id) { \
        if (prev_id == referencer##_InvalidId) { \
            list->list_first[list_order] = accessor##_GetNext(list, &list->obj_arr[delete_id]); \
        } \
        else { \
            accessor##_SetNext(list, &list->obj_arr[prev_id], accessor##_GetNext(list, &list->obj_arr[delete_id])); \
        } \
        return delete_id; \
    } \
    void static_list_type_name##StaticListSwitch(static_list_type_name##StaticList* list, id_offset_type list_order, id_offset_type prev_id, id_offset_type id, id_offset_type new_list_order) { \
        static_list_type_name##StaticListDelete(list, list_order, prev_id, id); \
        static_list_type_name##StaticListPush(list, new_list_order, id); \
    } \
    id_offset_type static_list_type_name##StaticListIteratorFirst(static_list_type_name##StaticList* list, id_offset_type list_order) { \
        return list->list_first[list_order]; \
    } \
    id_offset_type static_list_type_name##StaticListIteratorNext(static_list_type_name##StaticList* list, id_offset_type cur_id) { \
        return accessor##_GetNext(list, &list->obj_arr[cur_id]); \
    } \

#define LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER_InvalidId (-1)
#define LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_REFERENCER

#define LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_ACCESSOR_SetNext(LIST, ELEMENT, NEXT) ((ELEMENT)->next = NEXT)
#define LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_ACCESSOR_GetNext(LIST, ELEMENT, NEXT) ((ELEMENT)->next)
#define LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_ACCESSOR LIBYUC_CONTAINER_STATIC_LIST_DEFAULT_ACCESSOR

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_STATIC_LIST_H_