/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONTAINER_SIGNLY_LIST_H_
#define CUTILS_CONTAINER_SIGNLY_LIST_H_

#include "CUtils/object.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SinglyListEntry {
    struct _SinglyListEntry* next;
} SinglyListEntry;

typedef SinglyListEntry SinglyListHead;

void SinglyListHeadInit(SinglyListHead* head);
bool SinglyListIsEmpty(SinglyListHead* head);
void SinglyListInsertHead(SinglyListHead* head, SinglyListEntry* entry);
SinglyListEntry* SinglyListRemoveHead(SinglyListHead* head);
SinglyListEntry* SinglyListRemoveEntry(SinglyListEntry* prev, SinglyListEntry* removeEntry);
size_t SinglyListGetCount(SinglyListHead* head);
SinglyListEntry* SinglyListFirst(SinglyListHead* head);
SinglyListEntry* SinglyListNext(SinglyListEntry* entry);

#define SinglyListFindEntryByField(head, retObj, key, objName, entryFieldName, keyFieldName) { \
    retObj = NULL; \
    SinglyListEntry* cur = (head)->next; \
    while ((head) != cur) { \
        objName* tempObj = GetObjFromField(cur, objName, entryFieldName); \
        if (tempObj->keyFieldName == (key)) { \
            retObj = tempObj; \
            break; \
        } \
        cur = cur->next; \
    } \
}



#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONTAINER_SIGNLY_LIST_H_