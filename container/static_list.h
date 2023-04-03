/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_STATIC_LIST_H_
#define CUTILS_CONTAINER_STATIC_LIST_H_

#include <CUtils/object.h>
#include <CUtils/container/vector.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CUTILS_CONTAINER_STATIC_LIST_DECLARATION_1(static_list_type_name, id_type) \
    typedef struct _##static_list_type_name##StaticListEntry { \
        id_type next; \
    } static_list_type_name##StaticListEntry; \

#define CUTILS_CONTAINER_STATIC_LIST_DECLARATION_2(static_list_type_name, id_type, element_type, list_count, obj_arr_count) \
    typedef struct _##static_list_type_name##StaticList { \
        id_type list_first[list_count]; /* 最少应为1 */ \
        element_type obj_arr[obj_arr_count]; \
    } static_list_type_name##StaticList; \
    \
    void static_list_type_name##StaticListInit(static_list_type_name##StaticList* list, id_type count); \
    id_type static_list_type_name##StaticListPop(static_list_type_name##StaticList* list, id_type list_order); \
    void static_list_type_name##StaticListPush(static_list_type_name##StaticList* list, id_type list_order, id_type index); \
    id_type static_list_type_name##StaticListDelete(static_list_type_name##StaticList* list, id_type list_order, id_type prev_id, id_type delete_id); \
    void static_list_type_name##StaticListSwitch(static_list_type_name##StaticList* list, id_type list_order, id_type id, id_type new_list_order); \
    id_type static_list_type_name##StaticListIteratorFirst(static_list_type_name##StaticList* list, id_type list_order); \
    id_type static_list_type_name##StaticListIteratorNext(static_list_type_name##StaticList* list, id_type cur_id); \

#define CUTILS_CONTAINER_STATIC_LIST_DECLARATION(static_list_type_name, id_type, element_type, list_count, obj_arr_count) \
    CUTILS_CONTAINER_STATIC_LIST_DECLARATION_1(static_list_type_name, id_type) \
    CUTILS_CONTAINER_STATIC_LIST_DECLARATION_2(static_list_type_name, id_type, element_type, list_count, obj_arr_count) \

/*
* 访问器需要提供_GetNext、_SetNext方法
*/
#define CUTILS_CONTAINER_STATIC_LIST_DEFINE(static_list_type_name, id_type, element_type, referencer, accessor, list_count) \
    void static_list_type_name##StaticListInit(static_list_type_name##StaticList* list, id_type count) { \
        list->list_first[0] = 0; \
        id_type i = 0; \
        for (; i < count; i++) { \
            if (i + 1 == count) { \
                accessor##_SetNext(list, list->obj_arr[i], referencer##_InvalidId); \
            } \
            else { \
                accessor##_SetNext(list, list->obj_arr[i], i + 1); \
            } \
        } \
        \
        for (i = 1; i < list_count; i++) { \
            list->list_first[i] = referencer##_InvalidId; \
        } \
    } \
    id_type static_list_type_name##StaticListPop(static_list_type_name##StaticList* list, id_type list_order) { \
        if (list->list_first[list_order] == referencer##_InvalidId) { \
            return referencer##_InvalidId; \
        } \
        id_type index = list->list_first[list_order]; \
        list->list_first[list_order] = accessor##_GetNext(list, list->obj_arr[index]); \
        return index; \
    } \
    void static_list_type_name##StaticListPush(static_list_type_name##StaticList* list, id_type list_order, id_type index) { \
        accessor##_SetNext(list, list->obj_arr[index], list->list_first[list_order]); \
        list->list_first[list_order] = index; \
    } \
    id_type static_list_type_name##StaticListDelete(static_list_type_name##StaticList* list, id_type list_order, id_type prev_id, id_type delete_id) { \
        if (prev_id == referencer##_InvalidId) { \
            list->list_first[list_order] = accessor##_GetNext(list, list->obj_arr[delete_id]); \
        } \
        else { \
            accessor##_SetNext(list, list->obj_arr[prev_id], accessor##_GetNext(list, list->obj_arr[delete_id])); \
        } \
        return delete_id; \
    } \
    void static_list_type_name##StaticListSwitch(static_list_type_name##StaticList* list, id_type list_order, id_type prev_id, id_type id, id_type new_list_order) { \
        static_list_type_name##StaticListDelete(list, list_order, prev_id, id); \
        static_list_type_name##StaticListPush(list, new_list_order, id); \
    } \
    id_type static_list_type_name##StaticListIteratorFirst(static_list_type_name##StaticList* list, id_type list_order) { \
        return list->list_first[list_order]; \
    } \
    id_type static_list_type_name##StaticListIteratorNext(static_list_type_name##StaticList* list, id_type cur_id) { \
        return accessor##_GetNext(list, list->obj_arr[cur_id]); \
    } \


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_STATIC_LIST_H_