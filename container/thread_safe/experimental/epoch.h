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



#define SLOT_COUNT 64

typedef struct _EpochSlot {
  uint32_t epoch_num;
  bool active;    // 是否处于活动状态
  volatile int32_t used;
  ThreadId thread_id;
} EpochSlot;

bool EpochSlotAcquire(EpochSlot* slot) {
  if (AtomicInt32CompareExchange(&slot->used, 1, 0)) {
    return true;
  }
  return false;
}

void EpochSlotRelease(EpochSlot* slot) {
   release_assert(slot->used == 1, "");
  slot->used = 0;
}


typedef struct _Epoch {
  volatile uint32_t global_epoch_num;   // 0、1、2
  uint32_t entry_count;   // 计数，一定次数后在触发回收

  EpochSlot slot[SLOT_COUNT];
} Epoch;



typedef uint32_t EpochSlotId;

/*
* 基于lock - free静态链表进行EpochThreadBlock的分配，有新线程就注册
* 如果静态链表分配失败说明工作线程过多，选择阻塞策略是可行的
*/

void EpochInit(Epoch* epoch) {
  epoch->global_epoch_num = 0;
  for (EpochSlotId i = 0; i < SLOT_COUNT; i++) {
    epoch->slot[i].active = false;
    epoch->slot[i].used = 0;
    epoch->slot[i].epoch_num = 0;
  }
}

bool EpochTryGc(Epoch* epoch) {
  // 遍历block，如果没有epoch_num < global_epoch_num的block处于活动状态，则global_epoch_num++，将epoch_num < global_epoch_num的block中的垃圾队列的垃圾回收
  return true;
}


static void EpochRegisterThread(Epoch* epoch, EpochSlotId* slot_id) {
  // 分配slot
  for (EpochSlotId i = 0; i < SLOT_COUNT; i++) {
    if (EpochSlotAcquire(&epoch->slot[i])) {
      *slot_id = i;
      return;
    }
  }
  Painc("epoch slot full.");
}

static void EpochUnregisterThread(Epoch* epoch, EpochSlotId* slot_id) {
  EpochSlotRelease(&epoch->slot[*slot_id]);
}

void EpochEntry(Epoch* epoch) {
  static thread_local EpochSlotId thread_block_id = -1;
  if (thread_block_id == -1) {
    // 线程第一次调用Entry，注册
    EpochRegisterThread(epoch, &thread_block_id);
  }
}

void EpochLeave(Epoch* epoch) {
  // 取消注册
}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_EPOCH_H_