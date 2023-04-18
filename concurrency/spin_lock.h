/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
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
