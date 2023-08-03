/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONCURRENCY_RW_LOCK_H_
#define LIBYUC_CONCURRENCY_RW_LOCK_H_

#include <libyuc/object.h>
#include <libyuc/concurrency/mutex_lock.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 读写锁
*/
typedef struct _RwLock {
  AtomicInt32 read_count;
  MutexLock write_lock;
  AtomicBool write_wait;
} RwLock;

static forceinline void RwLockInit(RwLock* lock) {
  lock->read_count = 0;
  MutexLockInit(&lock->write_lock);
  lock->write_wait = false;
}

static forceinline void RwLockReadAcquire(RwLock* lock) {
  while (AtomicBoolLoad(&lock->write_wait)) { ThreadSwitch(); continue; }    // 防止写饥饿，一旦有写开始等待读完成，后来的读就不再参与抢占
  MutexLockAcquire(&lock->write_lock);    // 先抢占写锁
  // 随后增加读计数并释放写锁
  AtomicInt32Increment(&lock->read_count);    // 抢占了写锁依旧使用原子自增是因为可能存在另一个读正好释放读锁
  MutexLockRelease(&lock->write_lock);
}

static forceinline void RwLockReadRelease(RwLock* lock) {
  AtomicInt32Decrement(&lock->read_count);
}

static forceinline void RwLockWriteAcquire(RwLock* lock) {
  MutexLockAcquire(&lock->write_lock);    // 抢占写锁
  AtomicBoolStore(&lock->write_wait, true);
  while (AtomicInt32Load(&lock->read_count) != 0) { ThreadSwitch();  continue; }    // 等待正在进行的读完成
  AtomicBoolStore(&lock->write_wait, false);
}

static forceinline void RwLockWriteRelease(RwLock* lock) {
  MutexLockRelease(&lock->write_lock);
}

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONCURRENCY_RW_LOCK_H_