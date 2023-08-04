/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONCURRENCY_SPIN_LOCK_H_
#define LIBYUC_CONCURRENCY_SPIN_LOCK_H_

#include <libyuc/object.h>

#include <libyuc/concurrency/thread.h>
#include <libyuc/concurrency/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 自旋锁
*/
typedef struct _SpinLock {
  AtomicBool state;
} SpinLock;

static forceinline void SpinLockInit(SpinLock* lock) {
  lock->state = false;
}

static forceinline void SpinLockAcquire(SpinLock* lock) {
  while (AtomicBoolExchange(&lock->state, true) == true) { ThreadPause(); continue; }
}

static forceinline void SpinLockRelease(SpinLock* lock) {
  AtomicBoolStore(&lock->state, false);
}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONCURRENCY_SPIN_LOCK_H_
