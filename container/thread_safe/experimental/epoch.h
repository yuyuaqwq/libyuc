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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Epoch {
  volatile uint32_t global_epoch_num;   // 0、1、2
  uint32_t entry_count;   // 计数，一定次数后在触发回收
} Epoch;

typedef struct _EpochThreadBlock {
  uint32_t epoch_num : 31;
  uint32_t active : 1;    // 是否正在访问临界区
} EpochThreadBlock;

typedef uint32_t EpochThreadBlockId;

/*
* 基于lock - free静态链表进行EpochThreadBlock的分配，有新线程就注册
* 如果静态链表分配失败说明工作线程过多，选择阻塞策略
*/

bool EpochTryGc(Epoch* epoch) {

}

void EpochEntry(Epoch* epoch) {
  static thread_local EpochThreadBlockId thread_block_id = -1;
  if (thread_block_id == -1) {
    // 线程第一次调用Entry，注册
    thread_block_id = 0;
    // 线程死亡，如何回收注册的ThreadBlock？退出临界区的同时也将EpochThreadBlock挂到全局epoch中，
  }
}

void EpochLeave(Epoch* epoch) {

}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONTAINER_THREAD_SAFE_EPOCH_H_