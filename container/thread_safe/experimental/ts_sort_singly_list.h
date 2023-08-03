/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_THREAD_SAFE_TS_SORT_SINGLY_LIST_H_
#define LIBYUC_CONTAINER_THREAD_SAFE_TS_SORT_SINGLY_LIST_H_

/*
* Lock-Free SortSinglyList - 无锁有序单向链表
*/

#include <libyuc/object.h>

#include <libyuc/concurrency/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _TsSortSinglyListEntryInt{
  id_type next;
  int key;
} TsSortSinglyListEntryInt;

typedef struct _TsSortSinglyListEntry {
  struct _TsSortSinglyListEntry* next;
} TsSortSinglyListEntry;
typedef struct _TsSortSinglyListHead {
  TsSortSinglyListEntry* first;
} TsSortSinglyListHead;

void TsSortSinglyListHeadInit(TsSortSinglyListHead* head) {
  head->first = NULL;
}

#define IS_MARK(p) ((uintptr_t)(p) & 0x01)
#define MARK(p) ((TsSortSinglyListEntry*)((uintptr_t)(p) | 0x01))
#define CLEAR_MARK(p) ((TsSortSinglyListEntry*)((uintptr_t)(p) & ~0x01))

#ifndef TsSortSinglyListEntryGetKey
#define TsSortSinglyListEntryGetKey(ENTRY) (&((TsSortSinglyListEntryInt*)ENTRY)->key)
#endif

#ifndef TsSortSinglyListEntryGetNext
#define TsSortSinglyListEntryGetNext(ENTRY) ((ENTRY)->next)
#endif

#ifndef TsSortSinglyListEntryGetNextPtr
#define TsSortSinglyListEntryGetNextPtr(ENTRY) (&(ENTRY)->next)
#endif

#ifndef TsSortSinglyListEntrySetNext
#define TsSortSinglyListEntrySetNext(ENTRY, NEXT) (ENTRY)->next = (NEXT)
#endif


// 节点被逻辑删除了也能继续使用，因为没有被回收也就不会出现ABA问题

int TsSortSinglyListLocate(TsSortSinglyListEntry** prev_ptr, TsSortSinglyListEntry** cur_ptr, int key) {
  TsSortSinglyListEntry* cur = *cur_ptr;
  TsSortSinglyListEntry* prev = *prev_ptr;
  if (cur != NULL) {
    do {
      if (IS_MARK(cur)) {
        // 当前节点已经标记了删除，通过MARK标记的prev重试

        // cur可能指向head，此时key是无效的，因此不做断言
        // release_assert(*TsSortSinglyListEntryGetKey(CLEAR_MARK(cur)) <= *TsSortSinglyListEntryGetKey(prev), "%p:%d %d", cur, *TsSortSinglyListEntryGetKey(CLEAR_MARK(cur)), *TsSortSinglyListEntryGetKey(prev));
        prev = CLEAR_MARK(cur);
        cur = TsSortSinglyListEntryGetNext(prev);
        continue;
      }
      if (*TsSortSinglyListEntryGetKey(cur) >= key) {
        *cur_ptr = cur;
        *prev_ptr = prev;
        return *TsSortSinglyListEntryGetKey(cur) == key ? 0 : 1;
      }
      prev = cur;
      cur = TsSortSinglyListEntryGetNext(cur);
    } while (cur);
  }
  // 插入到末尾
  *prev_ptr = prev;
  *cur_ptr = NULL;
  return -1;
}

bool TsSortSinglyListInsert(TsSortSinglyListEntry* prev, TsSortSinglyListEntry* cur, TsSortSinglyListEntry* entry) {
  do {
    TsSortSinglyListLocate(&prev, &cur, *TsSortSinglyListEntryGetKey(entry));
    TsSortSinglyListEntrySetNext(entry, cur);
    // 需要避免prev是即将删除节点的场景，否则插入后prev即刻被删除(删除上下文中cur)会导致新插入节点丢失
    // 解决方法是DeleteCAS前将cur->next进行标记(末尾置为1)，此时当前的插入的CAS就会触发失败重试(prev->next被修改 != cur)
    if (AtomicPtrCompareExchange(TsSortSinglyListEntryGetNextPtr(prev), entry, cur)) {
      break;
    }
    // 更新失败的场景，即prev->next不再是cur
    // 1.prev被删除
    TsSortSinglyListEntry* next = TsSortSinglyListEntryGetNext(prev);
    if (IS_MARK(next)) {
      prev = CLEAR_MARK(next);
      next = TsSortSinglyListEntryGetNext(prev);
    }
    // 2.prev和cur之间插入了新节点
    cur = next;
  } while (true);
  return true;
}

bool TsSortSinglyListDeleteEntryInternal(TsSortSinglyListEntry** prev_ptr, TsSortSinglyListEntry** entry_ptr) {
  TsSortSinglyListEntry* prev = *prev_ptr;
  TsSortSinglyListEntry* entry = *entry_ptr;
   release_assert(!IS_MARK(prev), "");
   release_assert(!IS_MARK(entry), "");
  TsSortSinglyListEntry* next = TsSortSinglyListEntryGetNext(entry);
  // cur即将被删除，先进行标记，标记时将其置为prev使得TsSortSinglyListLocate可以往回找
  if (AtomicPtrCompareExchange(TsSortSinglyListEntryGetNextPtr(entry), MARK(prev), CLEAR_MARK(next))) {
    // 只有一个删除线程能成功标记这个节点

     release_assert(!next || *TsSortSinglyListEntryGetKey(next) >= *TsSortSinglyListEntryGetKey(entry), "");
    if (AtomicPtrCompareExchange(TsSortSinglyListEntryGetNextPtr(prev), next, entry)) {
      *entry_ptr = entry;
      return true;
    }
    // 删除失败的场景，即prev->next不再是cur
    // 1.prev将要被删除/已被删除，被打上标记
    if (IS_MARK(TsSortSinglyListEntryGetNext(prev))) {
      TsSortSinglyListEntrySetNext(entry, next);   // 此时cur是通过新的prev访问的(其他的删除线程将要/已经进行了prev的prev指向cur的原子操作)，先取消标记
      prev = CLEAR_MARK(TsSortSinglyListEntryGetNext(prev));    // 由此重试
      entry = TsSortSinglyListEntryGetNext(prev);
    }
    // 2.prev和cur之间插入了新节点
    else {
      TsSortSinglyListEntrySetNext(entry, next);
      prev = TsSortSinglyListEntryGetNext(prev);
    }
  }
  else {
    // 标记失败的场景，即cur->next不再是next
    // 1.cur被其他删除线程标记
    if (IS_MARK(TsSortSinglyListEntryGetNext(entry))) {
      prev = CLEAR_MARK(TsSortSinglyListEntryGetNext(entry));
    }
    // 2.cur->next已被其他线程删除
    entry = TsSortSinglyListEntryGetNext(prev);
  }
  *entry_ptr = entry;
  *prev_ptr = prev;
  return false;
}

bool TsSortSinglyListDelete(TsSortSinglyListEntry* prev, TsSortSinglyListEntry* cur, int key) {
  do {
    if (TsSortSinglyListLocate(&prev, &cur, key) != 0) {
      return false;
    }
    if (TsSortSinglyListDeleteEntryInternal(&prev, &cur)) {
      return true;
    }
  } while (true);
  return false;
}

void TsSortSinglyListDeleteEntry(TsSortSinglyListEntry* prev, TsSortSinglyListEntry* entry) {
  while (!TsSortSinglyListDeleteEntryInternal(&prev, &entry));
}

TsSortSinglyListEntry* TsSortSinglyListIteratorFirst(TsSortSinglyListHead* head) {
  return head->first;
}

TsSortSinglyListEntry* TsSortSinglyListIteratorNext(TsSortSinglyListEntry* cur) {
  do  {
    cur = TsSortSinglyListEntryGetNext(cur);
  } while (IS_MARK(cur));
  return cur;
}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_TS_SORT_SINGLY_LIST_H_