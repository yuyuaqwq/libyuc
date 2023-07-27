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
#include <libyuc/container/thread_safe/ts_sort_singly_list.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 跳表索引节点
*/
typedef struct _TsSkipListLevel {
  TsSortSinglyListEntry
} TsSkipListLevel;

/*
* 跳表节点，每个节点有随机的索引层数，所以用柔性数组动态分配
*/
typedef struct _SkipListEntry {
  int key;
  SkipListLevel upper[];    // 节点的上层，是索引节点
} SkipListEntry;

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_SKIP_LIST_H_