/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONCURRENCY_ATOMIC_H_
#define CUTILS_CONCURRENCY_ATOMIC_H_

#include <CUtils/object.h>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>

/*
* 原子操作
*/

/*
* 原子自增
*/
static inline int32_t AtomicIncrement32(volatile int32_t* target) {
	return InterlockedIncrement(target);
}
/*
* 原子自减
*/
static inline int32_t AtomicDecrement32(volatile int32_t* target) {
	return InterlockedDecrement(target);
}
/*
* 原子交换
* 修改target为value
* 返回target旧值
*/
static inline int32_t AtomicExchange32(volatile int32_t* target, int32_t value) {
	return InterlockedExchange(target, value);
}
/*
* 原子比较并交换
* 若target == comparand，则修改target为exchange
* 返回target旧值
*/
static inline int32_t AtomicCompareExchange32(volatile int32_t* target, int32_t exchange, int32_t comparand) {
	return InterlockedCompareExchange(target, exchange, comparand);
}
#endif


#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONCURRENCY_ATOMIC_H_