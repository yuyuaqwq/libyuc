/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef CUTILS_CONCURRENCY_SPIN_LOCK_H_
#define CUTILS_CONCURRENCY_SPIN_LOCK_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 自旋锁
*/
typedef struct _SpinLock {
	volatile bool state;
} SpinLock;

void SpinLockInit(SpinLock* lock);
void SpinLockAcquire(SpinLock* lock);
void SpinLockRelease(SpinLock* lock);

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONCURRENCY_SPIN_LOCK_H_
