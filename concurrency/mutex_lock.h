/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONCURRENCY_MUTEX_LOCK_H_
#define CUTILS_CONCURRENCY_MUTEX_LOCK_H_

#include <CUtils/object.h>

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

#endif // CUTILS_CONCURRENCY_MUTEX_LOCK_H_
