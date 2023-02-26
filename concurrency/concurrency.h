/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONCURRENCY_CONCURRENCY_H_
#define CUTILS_CONCURRENCY_CONCURRENCY_H_

#include <CUtils/object.h>


#ifdef __cplusplus
extern "C" {
#endif
/*
* 原子操作
*/
int32_t AtomicIncrement32(volatile int32_t* target);
int32_t AtomicDecrement32(volatile int32_t* target);
int32_t AtomicExchange32(volatile int32_t* target, int32_t value);
int32_t AtomicCompareExchange32(volatile int32_t* target, int32_t exchange, int32_t comparand);

/*
* 自旋锁
*/
typedef struct _SpinLock {
	volatile bool state;
} SpinLock;

void SpinLockInit(SpinLock* lock);
void SpinLockAcquire(SpinLock* lock);
void SpinLockRelease(SpinLock* lock);

/*
* 互斥锁(基于线程休眠)
*/
typedef struct _MutexLock {
	volatile bool state;
} MutexLock;

void MutexLockInit(MutexLock* lock);
void MutexLockAcquire(MutexLock* lock);
void MutexLockRelease(MutexLock* lock);

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

#endif // CUTILS_CONCURRENCY_CONCURRENCY_H_