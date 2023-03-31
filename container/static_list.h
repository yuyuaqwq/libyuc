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

#define CUTILS_CONTAINER_STATIC_LIST_DECLARATION(static_list_type_name, id_type, element_type, list_count, obj_arr_count) \
    /*
    * 若只有一条队列，由Pop分配的节点，此结构体可以被覆盖，可以使用union
    * StaticListEntry必须是偏移为0的字段
    */ \
    typedef struct _##static_list_type_name##StaticListEntry { \
        id_type next; \
    } static_list_type_name##StaticListEntry; \
    typedef struct _##static_list_type_name##StaticList { \
        id_type list_first[list_count]; /* 最少应为1 */ \
        element_type obj_arr[obj_arr_count]; \
    } static_list_type_name##StaticList; \
    \
    void static_list_type_name##StaticListInit(static_list_type_name##StaticList* list, id_type count); \
    id_type static_list_type_name##StaticListPop(static_list_type_name##StaticList* list, id_type list_order); \
    void static_list_type_name##StaticListPush(static_list_type_name##StaticList* list, id_type list_order, id_type index); \

#define CUTILS_CONTAINER_STATIC_LIST_DEFINE(static_list_type_name, id_type, element_type, referencer, list_count) \
    void static_list_type_name##StaticListInit(static_list_type_name##StaticList* list, id_type count) { \
        list->list_first[0] = 0; \
        id_type i = 0; \
        for (; i < count; i++) { \
            if (i + 1 == count) { \
                ((static_list_type_name##StaticListEntry*)&list->obj_arr[i])->next = referencer##_InvalidId; \
            } \
            else { \
                ((static_list_type_name##StaticListEntry*)&list->obj_arr[i])->next = i + 1; \
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
        list->list_first[list_order] = ((static_list_type_name##StaticListEntry*)&list->obj_arr[index])->next; \
        return index; \
    } \
    void static_list_type_name##StaticListPush(static_list_type_name##StaticList* list, id_type list_order, id_type index) { \
        ((static_list_type_name##StaticListEntry*)&list->obj_arr[index])->next = list->list_first[list_order]; \
        list->list_first[list_order] = index; \
    } \


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_STATIC_LIST_H_