/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_THREAD_SAFE_SORT_SINGLY_LIST_H_
#define LIBYUC_CONTAINER_THREAD_SAFE_SORT_SINGLY_LIST_H_

/*
* Lock-Free SortSinglyList - 无锁有序单向链表
*/

#include <libyuc/object.h>

#include <libyuc/concurrency/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _TsSortSinglyListEntry {
  struct _TsSortSinglyListEntry* next;
  int key;
} TsSortSinglyListEntry;
typedef struct _TsSortSinglyListHead {
  TsSortSinglyListEntry* first;
} TsSortSinglyListHead;

void TsSortSinglyListHeadInit(TsSortSinglyListHead* head) {
  head->first = NULL;
}

#define IS_MARK(p) ((uintptr_t)(p) & 0x01)
#define MARK(p) ((void*)((uintptr_t)(p) | 0x01))
#define CLEAR_MARK(p) ((uintptr_t)(p) & ~0x01)

int TsSortSinglyListLocate(TsSortSinglyListEntry** cur_ptr, TsSortSinglyListEntry** prev_ptr, int key) {
  TsSortSinglyListEntry* cur = *cur_ptr;
  TsSortSinglyListEntry* prev = *prev_ptr;
  do {
    if (cur->key >= key) {
      *cur_ptr = cur;
      *prev_ptr = prev;
      return cur->key == key ? 0 : 1;
    }
    prev = cur;
    cur = cur->next;
  } while (cur);
  // 插入到末尾
  *prev_ptr = prev;
  *cur_ptr = NULL;
  return -1;
}

bool TsSortSinglyListInsert(TsSortSinglyListEntry* prev, TsSortSinglyListEntry* cur, TsSortSinglyListEntry* entry) {
  do {
    TsSortSinglyListLocate(&cur, &prev, entry->key);
    entry->next = cur;
    // 需要避免prev是即将删除节点的场景，否则插入后prev即刻被删除(prev->next = cur->next)会导致节点丢失
    // 解决方法是DeleteCAS前将prev->next进行标记(末尾置为1)，此时当前的插入的CAS就会触发失败重试(prev->next拿到的指针的最低位为1，!= cur)
    if (AtomicCompareExchangePtr(&prev->next, entry, cur)) {
      break;
    }
    // 更新失败的场景，即prev->next不再是cur
    // 场景1：cur被删除
    // 场景2：prev和cur之间插入了新节点
    cur = prev;   // 由此开始重试，基于ebr，即便prev被逻辑删除了也能继续使用，因为没有被回收也就不会出现ABA问题
  } while (true);
  return true;
}

bool TsSortSinglyListDelete(TsSortSinglyListEntry* prev, TsSortSinglyListEntry* cur, int key) {
  do {
    if (TsSortSinglyListLocate(&cur, &prev, key) != 0) {
      return false;
    }
    TsSortSinglyListEntry* next = cur->next;
    // cur即将被删除，先进行标记
    if (AtomicCompareExchangePtr(&prev->next, MARK(cur), cur)) {
      // 成功标记
      if (AtomicCompareExchangePtr(&prev->next, next, cur)) {
        break;
      }
    }
    cur = prev;
  } while (true);
}


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_SORT_SINGLY_LIST_H_