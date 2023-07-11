/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONCURRENCY_MUTEX_LOCK_H_
#define LIBYUC_CONCURRENCY_MUTEX_LOCK_H_

#include <libyuc/object.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 互斥锁(基于线程切换)
*/
typedef struct _MutexLock {
	volatile bool state;
} MutexLock;

void MutexLockInit(MutexLock* lock);
void MutexLockAcquire(MutexLock* lock);
void MutexLockRelease(MutexLock* lock);

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONCURRENCY_MUTEX_LOCK_H_
