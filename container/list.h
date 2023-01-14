/*
* @yuyuaqwq - 鱼鱼
* emali:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_LIST_H_
#define CUTILS_CONTAINER_LIST_H_

#include "CUtils/object.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _ListEntry {
    struct _ListEntry* prev;
    struct _ListEntry* next;
} ListEntry;

typedef ListEntry ListHead;

void ListHeadInit(ListHead* head);
void ListEntryInit(ListEntry* entry);
bool ListIsEmpty(ListHead* entry);
void ListInsertHead(ListHead* head, ListEntry* entry);
void ListInsertTail(ListHead* head, ListEntry* entry);
ListEntry* ListRemoveEntry(ListEntry* entry, bool empty);
ListEntry* ListRemoveHead(ListHead* head);
ListEntry* ListRemoveTail(ListHead* head);
size_t ListGetEntryCount(ListHead* head);
#define ListFindEntryByKeyM(head, retObj, key, objName, entryFieldName, keyFieldName) { \
    retObj = NULL; \
    ListEntry* cur = (head)->next; \
    while ((head) != cur) { \
        objName* tempObj = GetObjByField(cur, objName, entryFieldName); \
        if (tempObj->keyFieldName == (key)) { \
            retObj = tempObj; \
            break; \
        } \
        cur = cur->next; \
    } \
}

ListEntry* ListFirst(ListHead* head);
ListEntry* ListPrev(ListHead* head, ListEntry* entry);
ListEntry* ListNext(ListHead* head, ListEntry* entry);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_LIST_H_