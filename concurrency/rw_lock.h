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
  volatile int32_t read_count;
  MutexLock write_lock;
  volatile bool write_wait;
} RwLock;

void RwLockInit(RwLock* lock);
void RwLockReadAcquire(RwLock* lock);
void RwLockReadRelease(RwLock* lock);
void RwLockWriteAcquire(RwLock* lock);
void RwLockWriteRelease(RwLock* lock);

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONCURRENCY_RW_LOCK_H_