/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONCURRENCY_ATOMIC_H_
#define LIBYUC_CONCURRENCY_ATOMIC_H_

#include <libyuc/object.h>

#if defined(_WIN32)
#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 原子操作
*/

/*
* 原子自增
*/
static forceinline int32_t AtomicIncrement32(volatile int32_t* target) {
  return (int32_t)InterlockedIncrement((volatile LONG*)target);
}

static forceinline int64_t AtomicIncrement64(volatile int64_t* target) {
  return (int64_t)InterlockedIncrement64((volatile LONG64*)target);
}

/*
* 原子自减
*/
static forceinline int32_t AtomicDecrement32(volatile int32_t* target) {
  return (int32_t)InterlockedDecrement((volatile LONG*)target);
}

static forceinline int64_t AtomicDecrement64(volatile int64_t* target) {
  return (int64_t)InterlockedDecrement64((volatile LONG64*)target);
}
/*
* 原子交换
* 修改target为value
* 返回target旧值
*/
static forceinline int32_t AtomicExchange32(volatile int32_t* target, int32_t value) {
  return (int32_t)InterlockedExchange((volatile LONG*)target, (LONG)value);
}

static forceinline int64_t AtomicExchange64(volatile int64_t* target, int64_t value) {
  return (int64_t)InterlockedExchange64((volatile LONG64*)target, (LONG64)value);
}

static forceinline void* AtomicExchangePtr(volatile void* target, void* value) {
#if defined(_WIN64)
  return (void*)AtomicExchange64((volatile int64_t*)target, (int64_t)value);
#else
  return (void*)AtomicExchange32((volatile int32_t*)target, (int32_t)value);
#endif
}

/*
* 原子比较并交换
* 若target == comparand，则修改target为exchange，否则不做处理
* 返回修改是否成功
*/
static forceinline bool AtomicCompareExchange32(volatile int32_t* target, int32_t exchange, int32_t comparand) {
  return InterlockedCompareExchange((volatile LONG*)target, (LONG)exchange, (LONG)comparand) == (LONG)comparand;
}

static forceinline bool AtomicCompareExchange64(volatile int64_t* target, int64_t exchange, int64_t comparand) {
  return InterlockedCompareExchange64((volatile LONG64*)target, (LONG64)exchange, (LONG64)comparand) == (LONG64)comparand;
}

static forceinline bool AtomicCompareExchangePtr(volatile void* target, void* exchange, void* comparand) {
#if defined(_WIN64)
  return AtomicCompareExchange64((volatile int64_t*)target, (int64_t)exchange, (int64_t)comparand);
#else
  return AtomicCompareExchange32((volatile int32_t*)target, (int32_t)exchange, (int32_t)comparand);
#endif
}


#endif


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONCURRENCY_ATOMIC_H_