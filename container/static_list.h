/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_STATIC_LIST_H_
#define CUTILS_CONTAINER_STATIC_LIST_H_

#include <CUtils/object.h>
#include <CUtils/container/array.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 若只有一条队列，Pop分配的节点，此节点的值可以被覆盖，可以使用union
*/
typedef struct _StaticListEntry {
    int next_index;
} StaticListEntry;


typedef struct _StaticList {
    Array array;
    int entry_field_offset;
    int list_first[1];
} StaticList;


extern const int kStaticListInvalidIndex;
void StaticListInit(StaticList* list, size_t count, int obj_size, int entry_field_offset, int list_count);
int StaticListPop(StaticList* list, int list_order);
void StaticListPush(StaticList* list, int list_order, int index);
#define StaticListAt(list, index, objName) (ArrayAt((list)->array, index, objName))

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_STATIC_LIST_H_