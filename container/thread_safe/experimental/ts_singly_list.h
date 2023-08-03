/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_THREAD_SAFE_SINGLY_LIST_H_
#define LIBYUC_CONTAINER_THREAD_SAFE_SINGLY_LIST_H_

/*
* Lock-Free TsSinglyList - 无锁单向链表
*/

#include <libyuc/object.h>

#include <libyuc/concurrency/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define id_type AtomicPtr(struct _TsSinglyListEntry)

typedef struct _TsSinglyListEntry {
  id_type next;
} TsSinglyListEntry;

typedef struct _TsSinglyListHead {
  id_type first;
} TsSinglyListHead;


void TsSinglyListHeadInit(TsSinglyListHead* head) {
  head->first = NULL;
}
bool TsSinglyListIsEmpty(TsSinglyListHead* head) {
  return head->first == NULL;
}
void TsSinglyListPutFirst(TsSinglyListHead* head, id_type entry) {
  do {
    entry->next = head->first;    /* 读取可能拿不到最新的first(读取后其他线程完成了插入的第二步的设置/删除了该节点) */
    /* head->first = entry; */
  } while (!AtomicPtrCompareExchange(&head->first, (void*)entry, (void*)entry->next));    /* 通过Cas保证entry->next依旧有效 */
}
void TsSinglyListDeleteFirst(TsSinglyListHead* head) {
  id_type old_first;
  do {
    old_first = head->first;    /* 两个线程可能读取到同一个节点/期间插入了新节点 */
    /* head->first = old_first->next; */
  } while (!AtomicPtrCompareExchange(&head->first, (void*)old_first->next, (void*)old_first));    /* 通过Cas保证只有一个线程能够摘除该节点/期间没有插入新节点 */
}

id_type TsSinglyListIteratorFirst(TsSinglyListHead* head) {
  return head->first;
}
id_type TsSinglyListIteratorNext(TsSinglyListHead* head, id_type entry) {
  id_type next_id = entry->next;
  return next_id;
}



#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_SINGLY_LIST_H_