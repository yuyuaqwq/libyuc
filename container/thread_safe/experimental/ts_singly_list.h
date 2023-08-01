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


#define LIBYUC_CONTAINER_THREAD_SAFE_TS_SINGLY_LIST_DECLARATION(ts_singly_list_type_name, id_type, offset_type) \
  typedef struct _##ts_singly_list_type_name##TsSinglyListEntry { \
    id_type next; \
  } ts_singly_list_type_name##TsSinglyListEntry; \
  typedef struct _##ts_singly_list_type_name##TsSinglyListHead { \
    id_type first; \
  } ts_singly_list_type_name##TsSinglyListHead; \
  \
  void ts_singly_list_type_name##TsSinglyListHeadInit(ts_singly_list_type_name##TsSinglyListHead* head); \
  bool ts_singly_list_type_name##TsSinglyListIsEmpty(ts_singly_list_type_name##TsSinglyListHead* head); \
  void ts_singly_list_type_name##TsSinglyListPutFirst(ts_singly_list_type_name##TsSinglyListHead* head, id_type entry_id); \
  void ts_singly_list_type_name##TsSinglyListDeleteFirst(ts_singly_list_type_name##TsSinglyListHead* head); \
  void ts_singly_list_type_name##TsSinglyListDeleteEntry(ts_singly_list_type_name##TsSinglyListHead* head, id_type prev_id, id_type del_entry_id); \
  offset_type ts_singly_list_type_name##TsSinglyListGetCount(ts_singly_list_type_name##TsSinglyListHead* head); \
  id_type ts_singly_list_type_name##TsSinglyListIteratorFirst(ts_singly_list_type_name##TsSinglyListHead* head); \
  id_type ts_singly_list_type_name##TsSinglyListIteratorNext(ts_singly_list_type_name##TsSinglyListHead* head, id_type entry_id); \

#define LIBYUC_CONTAINER_THREAD_SAFE_TS_SINGLY_LIST_DEFINE(ts_singly_list_type_name, id_type, id_atomic_type, referencer) \
  void ts_singly_list_type_name##TsSinglyListHeadInit(ts_singly_list_type_name##TsSinglyListHead* head) { \
    head->first = referencer##_InvalidId; \
  } \
  bool ts_singly_list_type_name##TsSinglyListIsEmpty(ts_singly_list_type_name##TsSinglyListHead* head) { \
    return head->first == referencer##_InvalidId; \
  } \
  void ts_singly_list_type_name##TsSinglyListPutFirst(ts_singly_list_type_name##TsSinglyListHead* head, id_type entry_id) { \
    ts_singly_list_type_name##TsSinglyListEntry* entry = referencer##_Reference(head, entry_id); \
    do { \
      entry->next = head->first;    /* 读取可能拿不到最新的first(读取后其他线程完成了插入的第二步的设置/删除了该节点) */ \
      /* head->first = entry; */ \
    } while (!AtomicCompareExchange##id_atomic_type(&head->first, entry, entry->next));    /* 通过Cas保证entry->next依旧有效 */ \
    referencer##_Dereference(head, entry); \
  } \
  void ts_singly_list_type_name##TsSinglyListDeleteFirst(ts_singly_list_type_name##TsSinglyListHead* head) { \
    id_type old_first; \
    do { \
      old_first = head->first;    /* 两个线程可能读取到同一个节点/期间插入了新节点 */ \
      /* head->first = old_first->next; */ \
    } while (!AtomicCompareExchange##id_atomic_type(&head->first, old_first->next, old_first));    /* 通过Cas保证只有一个线程能够摘除该节点/期间没有插入新节点 */ \
  } \
  offset_type ts_singly_list_type_name##TsSinglyListGetCount(ts_singly_list_type_name##TsSinglyListHead* head) { \
    offset_type count = 0; \
    id_type cur_id = head->first; \
    while (cur_id != referencer##_InvalidId) { \
      count++; \
      ts_singly_list_type_name##TsSinglyListEntry* cur = referencer##_Reference(head, cur_id); \
      cur_id = cur->next; \
      referencer##_Dereference(head, cur); \
    } \
    return count; \
  } \
  id_type ts_singly_list_type_name##TsSinglyListIteratorFirst(ts_singly_list_type_name##TsSinglyListHead* head) { \
    return head->first; \
  } \
  id_type ts_singly_list_type_name##TsSinglyListIteratorNext(ts_singly_list_type_name##TsSinglyListHead* head, id_type entry_id) { \
    ts_singly_list_type_name##TsSinglyListEntry* entry = referencer##_Reference(head, entry_id); \
    id_type next_id = entry->next; \
    referencer##_Dereference(head, entry); \
    return next_id; \
  } \



#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_SINGLY_LIST_H_