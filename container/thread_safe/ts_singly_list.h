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
  } while (AtomicCompareExchangePtr(&head->first, entry, entry->next) == head->first);    // 通过Cas保证entry->next依旧有效
}

TsSinglyListEntry* TsSinglyListDeleteFirst(TsSinglyListHead* head) {
  TsSinglyListEntry* old_first;
  do {
    old_first = head->first;    // 两个线程可能读取到同一个节点/期间插入了新节点
    // head->first = old_first->next;
  } while (AtomicCompareExchangePtr(&head->first, old_first->next, old_first) == head->first);    // 通过Cas保证只有一个线程能够摘除该节点/期间没有插入新节点
  return old_first;
}










#define LIBYUC_CONTAINER_SINGLY_LIST_DECLARATION(singly_list_type_name, id_type) \
  typedef struct _##singly_list_type_name##TsSinglyListEntry { \
    id_type next; \
  } singly_list_type_name##TsSinglyListEntry; \
  typedef struct _##singly_list_type_name##TsSinglyListHead { \
    id_type first; \
  } singly_list_type_name##TsSinglyListHead; \
  \
  void singly_list_type_name##TsSinglyListHeadInit(singly_list_type_name##TsSinglyListHead* head); \
  bool singly_list_type_name##TsSinglyListIsEmpty(singly_list_type_name##TsSinglyListHead* head); \
  void singly_list_type_name##TsSinglyListPutFirst(singly_list_type_name##TsSinglyListHead* head, id_type entry_id); \
  id_type singly_list_type_name##TsSinglyListDeleteFirst(singly_list_type_name##TsSinglyListHead* head); \
  id_type singly_list_type_name##TsSinglyListDeleteEntry(singly_list_type_name##TsSinglyListHead* head, id_type prev_id, id_type del_entry_id); \
  size_t singly_list_type_name##TsSinglyListGetCount(singly_list_type_name##TsSinglyListHead* head); \
  id_type singly_list_type_name##TsSinglyListIteratorFirst(singly_list_type_name##TsSinglyListHead* head); \
  id_type singly_list_type_name##TsSinglyListIteratorNext(singly_list_type_name##TsSinglyListHead* head, id_type entry_id); \

#define LIBYUC_CONTAINER_SINGLY_LIST_DEFINE(singly_list_type_name, id_type, referencer) \
  void singly_list_type_name##TsSinglyListHeadInit(singly_list_type_name##TsSinglyListHead* head) { \
    head->first = referencer##_InvalidId; \
  } \
  bool singly_list_type_name##TsSinglyListIsEmpty(singly_list_type_name##TsSinglyListHead* head) { \
    return head->first == referencer##_InvalidId; \
  } \
  void singly_list_type_name##TsSinglyListPutFirst(singly_list_type_name##TsSinglyListHead* head, id_type entry_id) { \
    singly_list_type_name##TsSinglyListEntry* entry = referencer##_Reference(head, entry_id); \
    entry->next = head->first; \
    head->first = entry_id; \
    referencer##_Dereference(head, entry); \
  } \
  id_type singly_list_type_name##TsSinglyListDeleteFirst(singly_list_type_name##TsSinglyListHead* head) { \
    id_type old_first_id = head->first; \
    singly_list_type_name##TsSinglyListEntry* old_first = referencer##_Reference(head, old_first_id); \
    head->first = old_first->next; \
    referencer##_Dereference(head, old_first_id); \
    return old_first_id; \
  } \
  id_type singly_list_type_name##TsSinglyListDeleteEntry(singly_list_type_name##TsSinglyListHead* head, id_type prev_id, id_type del_entry_id) { \
    singly_list_type_name##TsSinglyListEntry* prev; \
    if (prev_id != referencer##_InvalidId) { \
      prev = referencer##_Reference(head, prev_id); \
    } \
    else { \
      prev = (singly_list_type_name##TsSinglyListEntry*)head; \
    } \
    singly_list_type_name##TsSinglyListEntry* del_entry = referencer##_Reference(head, del_entry_id); \
    prev->next = del_entry->next; \
    if (prev_id != referencer##_InvalidId) { \
      referencer##_Dereference(head, prev); \
    } \
    referencer##_Dereference(head, del_entry); \
    return del_entry_id; \
  } \
  size_t singly_list_type_name##TsSinglyListGetCount(singly_list_type_name##TsSinglyListHead* head) { \
    size_t count = 0; \
    id_type cur_id = head->first; \
    while (cur_id != referencer##_InvalidId) { \
      count++; \
      singly_list_type_name##TsSinglyListEntry* cur = referencer##_Reference(head, cur_id); \
      cur_id = cur->next; \
      referencer##_Dereference(head, cur); \
    } \
    return count; \
  } \
  id_type singly_list_type_name##TsSinglyListIteratorFirst(singly_list_type_name##TsSinglyListHead* head) { \
    return head->first; \
  } \
  id_type singly_list_type_name##TsSinglyListIteratorNext(singly_list_type_name##TsSinglyListHead* head, id_type entry_id) { \
    singly_list_type_name##TsSinglyListEntry* entry = referencer##_Reference(head, entry_id); \
    id_type next_id = entry->next; \
    referencer##_Dereference(head, entry); \
    return next_id; \
  } \


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_SINGLY_LIST_H_