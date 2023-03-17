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


#define CUTILS_CONTAINER_STATIC_LIST_DECLARATION(static_list_type_name, element_type) \
    CUTILS_CONTAINER_VECTOR_DECLARATION(static_list_type_name, element_type) \
    /*
    * 若只有一条队列，Pop分配的节点，此节点的值可以被覆盖，可以使用union
    */ \
    typedef struct _StaticListEntry { \
        ptrdiff_t next_index; \
    } StaticListEntry; \
    typedef struct _StaticList { \
        static_list_type_name##Vector arr; \
        ptrdiff_t list_first[1]; \
    } StaticList; \


#define CUTILS_CONTAINER_STATIC_LIST_DEFINE(static_list_type_name, element_type, allocator) \
    CUTILS_CONTAINER_VECTOR_DEFINE(static_list_type_name, element_type, allocator) \


//extern const int kStaticListInvalidIndex;
//void StaticListInit(StaticList* list, size_t count, int obj_size, int entry_field_offset, int list_count);
//void StaticListInitFromBuf(StaticList* list, void* buf, size_t count, int obj_size, int entry_field_offset, int list_count);
//int StaticListPop(StaticList* list, int list_order);
//void StaticListPush(StaticList* list, int list_order, int index);
//#define StaticListAt(list, index, objName) (ArrayAt((list)->array, index, objName))

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_STATIC_LIST_H_