/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_DOUBLY_STATIC_LIST_H_
#define CUTILS_CONTAINER_DOUBLY_STATIC_LIST_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CUTILS_CONTAINER_DOUBLY_STATIC_LIST_DECLARATION(ds_list_type_name, id_type, element_type, list_count, obj_arr_count) \
    /*
    * 若只有一条队列，由Pop分配的节点，此结构体可以被覆盖，可以使用union
    */ \
    typedef struct _##ds_list_type_name##DoublyStaticListEntry{ \
        id_type prev; \
        id_type next; \
    } ds_list_type_name##DoublyStaticListEntry; \
    typedef struct _##ds_list_type_name##DoublyStaticList{ \
        id_type list_first[list_count]; /* 最少应为1 */ \
        element_type obj_arr[obj_arr_count]; \
    } ds_list_type_name##DoublyStaticList; \
    \
    void ds_list_type_name##DoublyStaticListInit(ds_list_type_name##DoublyStaticList* list, id_type count); \
    id_type ds_list_type_name##DoublyStaticListPop(ds_list_type_name##DoublyStaticList* list, id_type list_order); \
    void ds_list_type_name##DoublyStaticListPush(ds_list_type_name##DoublyStaticList* list, id_type list_order, id_type index); \

#define CUTILS_CONTAINER_DOUBLY_STATIC_LIST_DEFINE(ds_list_type_name, id_type, element_type, referencer, list_count) \
    void ds_list_type_name##DoublyStaticListInit(ds_list_type_name##DoublyStaticList* list, id_type count) { \
        list->list_first[0] = 0; \
        id_type i = 0; \
        for (; i < count; i++) { \
            if (i + 1 == count) { \
                list->obj_arr[i].next = referencer##_InvalidId; \
            } \
            else { \
                list->obj_arr[i].next = i + 1; \
            } \
            if (i == 0) { \
                list->obj_arr[i].prev = referencer##_InvalidId; \
            } \
            else { \
                list->obj_arr[i].prev = i - 1; \
            } \
        } \
        \
        for (i = 1; i < list_count; i++) { \
            list->obj_arr[i].list_first[i] = referencer##_InvalidId; \
        } \
    } \
    id_type ds_list_type_name##DoublyStaticListRemove(ds_list_type_name##DoublyStaticList* list, id_type list_order, id_type remove_index) { \
        if (list->obj_arr[remove_index].prev == referencer##_InvalidId) { \
            list->list_first[list_order] = list->obj_arr[remove_index].next; \
        } \
        else { \
            list->obj_arr[list->obj_arr[remove_index].prev].next = list->obj_arr[remove_index].next; \
        } \
        \
        if (list->obj_arr[remove_index].next != referencer##_InvalidId) { \
            list->obj_arr[list->obj_arr[remove_index].next].prev = list->obj_arr[remove_index].prev; \
        } \
        return remove_index; \
    } \
    id_type ds_list_type_name##DoublyStaticListPop(DoublyStaticList* list, id_type list_order) { \
        if (list->list_first[list_order] == referencer##_InvalidId) { \
            return referencer##_InvalidId; \
        } \
        return DoublyStaticListRemove(list, list_order, list->list_first[list_order]); \
    } \
    void ds_list_type_name##DoublyStaticListInsert(ds_list_type_name##DoublyStaticList* list, id_type list_order, id_type prev_index, id_type insert_index) { \
        if (prev_index == referencer##_InvalidId) { \
            list->obj_arr[insert_index].prev = referencer##_InvalidId; \
            list->obj_arr[insert_index].next = list->list_first[list_order]; \
            list->list_first[list_order] = insert_index; \
        } \
        else { \
            list->obj_arr[insert_index].prev = prev_index; \
            list->obj_arr[insert_index].next = list->obj_arr[prev_index].next; \
            list->obj_arr[prev_index].next = insert_index; \
        } \
        if (list->obj_arr[insert_index].next != referencer##_InvalidId) { \
            list->obj_arr[list->obj_arr[insert_index].next].prev = insert_index; \
        } \
    } \
    void ds_list_type_name##DoublyStaticListPush(ds_list_type_name##DoublyStaticList* list, id_type list_order, id_type push_index) { \
         ds_list_type_name##DoublyStaticListInsert(list, list_order, referencer##_InvalidId, push_index); \
    } \
    void ds_list_type_name##DoublyStaticListSwitch(ds_list_type_name##DoublyStaticList* list, id_type list_order, id_type index, id_type new_list_order) { \
        DoublyStaticListRemove(list, list_order, index); \
        DoublyStaticListPush(list, new_list_order, index); \
    } \

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_DOUBLY_STATIC_LIST_H_