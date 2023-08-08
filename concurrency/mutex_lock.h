/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONCURRENCY_MUTEX_LOCK_H_
#define LIBYUC_CONCURRENCY_MUTEX_LOCK_H_

#include <libyuc/basic.h>

#include <libyuc/concurrency/thread.h>
#include <libyuc/concurrency/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 互斥锁(基于线程切换)
*/
typedef struct _MutexLock {
  AtomicBool state;
} MutexLock;

static forceinline void MutexLockInit(MutexLock* lock) {
  lock->state = false;
}
static forceinline void MutexLockAcquire(MutexLock* lock) {
  while (AtomicBoolExchange(&lock->state, true) == true) { ThreadSwitch(); continue; }
}

static forceinline void MutexLockRelease(MutexLock* lock) {
  AtomicBoolStore(&lock->state, true);
}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONCURRENCY_MUTEX_LOCK_H_
