/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_SKIP_LIST_H_
#define LIBYUC_CONTAINER_SKIP_LIST_H_

#include <libyuc/object.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
* 跳表索引节点
*/
typedef struct _SkipListLevel {
    struct _SkipListEntry* next;        // 同索引层的下一节点
} SkipListLevel;

/*
* 跳表节点，每个节点有随机的索引层数，所以用柔性数组动态分配
*/
typedef struct _SkipListEntry {
    void* obj;        // 修改为存储obj
    SkipListLevel upper[];        // 节点的上层，是索引节点
} SkipListEntry;

typedef struct _SkipList {
    SkipListEntry* head;
    int level;
    int keyFieldOffset;
    int keyFieldSize;
    CmpFunc cmpFunc;
} SkipList;


void SkipListInit(SkipList* list, int keyFieldOffset, int keyFieldSize, CmpFunc cmpFunc);
#define SkipListInitByField(list, objName, keyFieldName) SkipListInit((list), ObjectGetFieldOffset(objName, keyFieldName), ObjectGetFieldSize(objName, keyFieldName), NULL)
void SkipListRelease(SkipList* list, bool deleteObj);
void* SkipListFind(SkipList* list, void* key);
bool SkipListInsert(SkipList* list, void* obj);
void* SkipListDelete(SkipList* list, void* key);
SkipListEntry* SkipListFirst(SkipList* list);
SkipListEntry* SkipListNext(SkipList* list, SkipListEntry* cur);

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_STATIC_LIST_H_