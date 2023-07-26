/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_THREAD_SAFE_SINGLY_LIST_H_
#define LIBYUC_CONTAINER_THREAD_SAFE_SINGLY_LIST_H_

/*
* Lock-Free SinglyList - 无锁单向链表
*/

#include <libyuc/object.h>

#include <libyuc/concurrency/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _TsSinglyListEntry {
  struct _TsSinglyListEntry* next;
} TsSinglyListEntry;
typedef struct _TsSinglyListHead {
  TsSinglyListEntry* first;
} TsSinglyListHead;

void TsSinglyListHeadInit(TsSinglyListHead* head) {
  head->first = NULL;
}

void TsSinglyListPutFirst(TsSinglyListHead* head, TsSinglyListEntry* entry) {
  do {
    entry->next = head->first;    // 读取可能拿不到最新的first(读取后其他线程完成了插入的第二步的设置/删除了该节点)
    // head->first = entry;
  } while (!AtomicCompareExchangePtr(&head->first, entry, entry->next));    // 通过Cas保证entry->next依旧有效
}

TsSinglyListEntry* TsSinglyListDeleteFirst(TsSinglyListHead* head) {
  TsSinglyListEntry* old_first;
  do {
    old_first = head->first;    // 两个线程可能读取到同一个节点/期间插入了新节点
    // head->first = old_first->next;
  } while (!AtomicCompareExchangePtr(&head->first, old_first->next, old_first));    // 通过Cas保证只有一个线程能够摘除该节点/期间没有插入新节点
  return old_first;
}


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_SINGLY_LIST_H_