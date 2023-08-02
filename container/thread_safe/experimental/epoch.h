/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONTAINER_THREAD_SAFE_EPOCH_H_
#define LIBYUC_CONTAINER_THREAD_SAFE_EPOCH_H_

/*
* Epoch-based reclame - 基于世代的垃圾回收算法
*/

#include <libyuc/object.h>

#include <libyuc/concurrency/atomic.h>
#include <libyuc/concurrency/tls.h>
#include <libyuc/concurrency/thread.h>

#include <libyuc/container/thread_safe/experimental/ts_singly_list.h>

#ifdef __cplusplus
extern "C" {
#endif


LIBYUC_CONTAINER_THREAD_SAFE_TS_SINGLY_LIST_DECLARATION(, struct _TsSinglyListEntry*, size_t)
LIBYUC_CONTAINER_THREAD_SAFE_TS_SINGLY_LIST_DEFINE(, struct _TsSinglyListEntry* , size_t, Ptr, LIBYUC_OBJECT_REFERENCER_DEFALUT)


typedef struct _EpochThreadBlock {
  uint32_t epoch_num;
  bool active;    // 是否处于活动状态
} EpochThreadBlock;

typedef struct _Epoch {
  volatile uint32_t global_epoch_num;   // 0、1、2
  uint32_t entry_count;   // 计数，一定次数后在触发回收

  EpochThreadBlock block[64];
} Epoch;



typedef uint32_t EpochThreadBlockId;

/*
* 基于lock - free静态链表进行EpochThreadBlock的分配，有新线程就注册
* 如果静态链表分配失败说明工作线程过多，选择阻塞策略是可行的
*/

void EpochInit(Epoch* epoch) {
  epoch->global_epoch_num = 0;
  for (size_t i = 0; i < sizeof(epoch->block) / sizeof(EpochThreadBlock); i++) {
    epoch->block[i].active = false;
    epoch->block[i].epoch_num = 0;
  }
}

bool EpochTryGc(Epoch* epoch) {
  // 遍历block，如果没有epoch_num < global_epoch_num的block处于活动状态，则global_epoch_num++，将epoch_num < global_epoch_num的block中的垃圾队列的垃圾回收
  return true;
}


static EpochThreadBlockId EpochRegisterThread(Epoch* epoch) {
  // 分配一个block
}

void EpochEntry(Epoch* epoch) {
  static thread_local EpochThreadBlockId thread_block_id = -1;
  if (thread_block_id == -1) {
    // 线程第一次调用Entry，注册


    
  }
}

void EpochLeave(Epoch* epoch) {
  // 取消注册
}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_EPOCH_H_