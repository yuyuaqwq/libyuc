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

#define TsSortSinglyListEntryGetKey TsSortSinglyListEntryGetKey_
extern "C" int* TsSortSinglyListEntryGetKey_(struct _TsSortSinglyListEntry* entry);
#include <libyuc/container/thread_safe/ts_sort_singly_list.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL 32
const int LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_SKIPLIST_P = (RAND_MAX / 2);
/*
* 跳表索引节点
*/
typedef TsSortSinglyListEntry TsSkipListLevel;

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



int* TsSortSinglyListEntryGetKey_(struct _TsSortSinglyListEntry* entry) {
  while (*(uintptr_t*)&--entry <= LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL);
  TsSkipListEntry* skip_list_entry = ObjectGetFromField(entry, TsSkipListEntry, level_count);
  return &skip_list_entry->key;
}


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

static forceinline TsSkipListEntry* TsSkipListLocate(TsSkipList* list, int key, int* cmp, TsSkipListLevel** update) {
  *cmp = 0;
  TsSkipListLevel* cur = list->head;
  // 从最顶层开始遍历，每趟循环都相当于下降一层索引
  for (int i = list->level - 1; i >= 0; i--) {
    // 当前索引层的链表查找
    TsSortSinglyListEntry* prev = &cur[i];
    TsSortSinglyListEntry* next = cur[i].next;
    *cmp = TsSortSinglyListLocate(&prev, &next, key);
    cur = prev;
    if (update) {
      update[i] = cur;    // 当前节点该层的索引可能需要 指向被删除索引的下一索引 / 指向新节点同层索引
    }
    // 查找到相等节点也要继续下降，因为没有提供prev指针，无法确定cur->upper[0]的上一节点
  }
  return ObjectGetFromField(cur, TsSkipListEntry, upper);
}

bool TsSkipListFind(TsSkipList* list, int key) {
  int cmp;
  TsSkipListEntry* cur = TsSkipListLocate(list, key, &cmp, NULL);

  cur = (TsSkipListEntry*)TsSortSinglyListIteratorNext((TsSortSinglyListEntry*)&cur->upper[0]);    // 刚出循环时cur还未更新
  // 查找出来，要么是所有索引层都找不到，要么是cur的key>=查找的key

  if (cur && cmp == 0) {
    return true;
  }
  return false;
}

bool TsSkipListInsert(TsSkipList* list, int key) {
  TsSkipListLevel* update[LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_MAX_LEVEL];    // 对应每一层需要更新索引的节点，因为新节点可能会插入索引

  int cmp;
  TsSkipListEntry* cur = TsSkipListLocate(list, key, &cmp, update);

  // cur此时的next要么指向NULL，要么>=key
  TsSkipListEntry* next = (TsSkipListEntry*)TsSortSinglyListIteratorNext((TsSortSinglyListEntry*)&cur->upper[0]);
  

  // 准备创建随机高度索引层的节点
  int level = TsRandomLevel();
  if (level > list->level) {
    // 新节点的索引层比以往的节点都高，高出来的部分由头节点索引层连接
    for (int i = list->level; i < level; i++) {
      update[i] = list->head;    // 头节点该层的索引需要指向新节点同层索引
    }
    list->level = level;
  }

  // 创建节点
  TsSkipListEntry* new_entry = TsSkipListCreateEntry(level, key);
  for (int i = 0; i < level; i++) {
    // 连接到同层索引链表中
    new_entry->upper[i].next = update[i][i].next;
    update[i][i].next = &new_entry->upper[i];
  }

  return true;
}



#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_H_