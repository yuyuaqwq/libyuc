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
    int list_first[1];
} DoublyStaticList;


extern const int kDoublyStaticListInvalidIndex;

void DoublyStaticListInit(DoublyStaticList* list, size_t count, int obj_size, int entry_field_offset, int list_count);
void DoublyStaticListInitFromBuf(DoublyStaticList* list, void* buf, size_t count, int obj_size, int entry_field_offset, int list_count);
void DoublyStaticListInsertNext(DoublyStaticList* list, int list_order, int prev_index, int insert_index);
int DoublyStaticListRemove(DoublyStaticList* list, int list_order, int remove_index);
void DoublyStaticListPush(DoublyStaticList* list, int list_order, int push_index);
int DoublyStaticListPop(DoublyStaticList* list, int list_order);
void DoublyStaticListSwitch(DoublyStaticList* list, int list_order, int index, int new_list_order);
#define DoublyStaticListAt(list, index, objName) (ArrayAt(&(list)->array, index, objName))

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_DOUBLY_STATIC_LIST_H_