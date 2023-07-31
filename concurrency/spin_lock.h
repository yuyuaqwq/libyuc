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
  volatile bool state;
} SpinLock;

static forceinline void SpinLockInit(SpinLock* lock) {
  lock->state = false;
}

static forceinline void SpinLockAcquire(SpinLock* lock) {
  while (AtomicExchange32(&lock->state, true) == true) { ThreadPause(); continue; }
}

static forceinline void SpinLockRelease(SpinLock* lock) {
  lock->state = false;    // 无需通过原子指令，只需要保证state是volatile就不会被编译器优化影响，最终会在某一时刻写回内存，原子性交给CPU
}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONCURRENCY_SPIN_LOCK_H_
