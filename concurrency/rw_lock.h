/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONCURRENCY_RW_LOCK_H_
#define CUTILS_CONCURRENCY_RW_LOCK_H_

#include <CUtils/object.h>
#include <CUtils/concurrency/mutex_lock.h>


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

#endif // CUTILS_CONCURRENCY_RW_LOCK_H_