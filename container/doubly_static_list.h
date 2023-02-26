/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_DOUBLY_STATIC_LIST_H_
#define CUTILS_CONTAINER_DOUBLY_STATIC_LIST_H_

#include <CUtils/object.h>
#include <CUtils/container/array.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 若只有一条队列，Pop分配的节点，此节点的值可以被覆盖，可以使用union
*/
typedef struct _DoublyStaticListEntry {
    int prev_index;
    int next_index;
} DoublyStaticListEntry;

typedef struct _DoublyStaticList {
    Array array;
    int entry_field_offset;
    int list_first[];
} DoublyStaticList;


extern const int kDoublyStaticListInvalidIndex;

void DoublyStaticListInit(DoublyStaticList* list, size_t count, int obj_size, int entry_field_offset, int list_count);
int DoublyStaticListEntryRemove(DoublyStaticList* list, int list_order, int remove_index);
void DoublyStaticListEntryInsertNext(DoublyStaticList* list, int list_order, int prev_index, int insert_index);
int DoublyStaticListEntryPop(DoublyStaticList* list, int list_order);
void DoublyStaticListEntryPush(DoublyStaticList* list, int list_order, int push_index);
#define DoublyStaticListAt(list, index, objName) (ArrayAt((list)->array, index, objName))

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_DOUBLY_STATIC_LIST_H_