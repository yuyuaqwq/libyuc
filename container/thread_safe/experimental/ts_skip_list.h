/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_H_
#define LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_H_

/*
* Lock-Free SkipList - 无锁跳表
*/
#include <libyuc/object.h>

#include <libyuc/concurrency/atomic.h>


#ifdef __cplusplus
extern "C" {
#endif

#define LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL 32
const int LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_SKIPLIST_P = (RAND_MAX / 2);
/*
* 跳表索引节点
*/
typedef struct _TsSkipListLevel {
  struct _TsSkipListEntry* next;    // 指向的同索引层的下一个SkipListEntry(不是直接指向逻辑上的同层链表)
} TsSkipListLevel;

/*
* 跳表节点，每个节点有随机的索引层数，所以用柔性数组动态分配
*/
typedef struct _TsSkipListEntry {
  int key;
  uintptr_t level_count;    // 也是阻断标记，根据是否小于等于LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL确定
  TsSkipListLevel upper[];    // 节点的上层，是索引节点
} TsSkipListEntry;


typedef struct _TsSkipList {
  TsSkipListLevel head[LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL];
  int level;
} TsSkipList;


static int TsRandomLevel() {
  int level = 1;
  unsigned int number;
  rand_s(&number);
  number %= RAND_MAX;
  while (LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_SKIPLIST_P <= number && level < LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL) {
    level++;
    rand_s(&number);
    number %= RAND_MAX;
  }
  return level;
}

void TsSkipListInit(TsSkipList* list) {
  list->level = 1;
  for (int i = 0; i < LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL; i++) {
    list->head[i].next = NULL;
  }
}

static TsSkipListEntry* TsSkipListCreateEntry(int level, int key) {
  TsSkipListEntry* entry = (TsSkipListEntry*)MemoryAlloc(sizeof(TsSkipListEntry) + level * sizeof(TsSkipListLevel));
  entry->key = key;
  entry->level_count = level;
  return entry;
}

static forceinline TsSkipListEntry* TsSkipListLocate(TsSkipList* list, int level, int key, int* cmp, TsSkipListLevel** update, TsSkipListEntry** update_next) {
  *cmp = 0;
  TsSkipListLevel* cur = list->head;
  // 从最顶层开始遍历，每趟循环都相当于下降一层索引
  for (int i = level - 1; i >= 0; i--) {
    // 当前索引层的链表查找
    TsSkipListEntry* next = cur[i].next;
    while (next) {
      *cmp = next->key - key;
      if (*cmp >= 0) {
        break;
      }
      cur = next->upper;   // 转到当前层的下一链表节点
      next = cur[i].next;
    }
    if (update) {
      update[i] = cur;    // 当前节点该层的索引可能需要 指向被删除索引的下一索引 / 指向新节点同层索引
      update_next[i] = next;
    }
    // 查找到相等节点也要继续下降，因为没有提供prev指针，无法确定cur->upper[0]的上一节点
  }
  return ObjectGetFromField(cur, TsSkipListEntry, upper);
}

bool TsSkipListFind(TsSkipList* list, int key) {
  int cmp;
  TsSkipListEntry* prev = TsSkipListLocate(list, list->level, key, &cmp, NULL, NULL);
  if (prev->upper[0].next && cmp == 0) {
    return true;
  }
  return false;
}

bool TsSkipListInsert(TsSkipList* list, int key) {
  TsSkipListLevel* update[LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL];    // 对应每一层需要更新索引的节点，因为新节点可能会插入索引
  TsSkipListEntry* update_next[LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL];

  int cmp;
  int level = list->level;
  TsSkipListEntry* prev = TsSkipListLocate(list, level, key, &cmp, update, update_next);

  // cur此时的next要么指向NULL，要么>=key
  TsSkipListEntry* cur = prev->upper[0].next;

  // 准备创建随机高度索引层的节点
  int new_level = TsRandomLevel();
  if (new_level > level) {
    // 新节点的索引层比以往的节点都高，高出来的部分由头节点索引层连接
    for (int i = level; i < new_level; i++) {
      update[i] = list->head;    // 头节点该层的索引需要指向新节点同层索引
    }
    while (!AtomicInt32CompareExchange(&list->level, new_level, level)) {
      if (new_level <= list->level) break;
      level = list->level;
    }
  }

  TsSkipListEntry* new_entry = TsSkipListCreateEntry(new_level, key);
  for (int i = 0; i < new_level; i++) {
    prev = ObjectGetFromField(update[i], TsSkipListEntry, upper);
    cur = update_next[i];
    do {
      new_entry->upper[i].next = cur;
      if (AtomicPtrCompareExchange(&prev->upper[i].next, new_entry, cur)) {
        break;
      }
      // 更新失败的场景
      // 1.update和update_next之间插入了新节点，重新定位合适的插入点
      cur = prev->upper[i].next;
      while (cur && cur->key < key) {
        prev = cur;
        cur = cur->upper[i].next;
      }
    } while (true);
  }
  return true;
}


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_H_