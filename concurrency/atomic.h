/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONCURRENCY_ATOMIC_H_
#define LIBYUC_CONCURRENCY_ATOMIC_H_

#include <libyuc/basic.h>
#include <libyuc/concurrency/memory_order.h>

#if defined(_WIN32)
#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 原子操作
*/

typedef volatile bool AtomicBool;
typedef volatile int32_t AtomicInt32;
typedef volatile uint32_t AtomicUint32;
typedef volatile int64_t AtomicInt64;
typedef volatile uint64_t AtomicUint64;
#define AtomicPtr(type) volatile type*

/*
* 原子读/写
* 单读单写的原子操作需要通过内存序确保正确性
*/

static forceinline bool AtomicBoolLoad(AtomicBool* target) {
    return *(volatile bool*)target;
}

static forceinline bool AtomicBoolLoadByMemoryOrder(AtomicBool* target, MemoryOrder order) {
    return *(volatile bool*)target;
}

static forceinline bool AtomicBoolStore(AtomicBool* target, bool val) {
    *target = (AtomicBool)val;
}

static forceinline bool AtomicBoolStoreByMemoryOrder(AtomicBool* target, bool val, MemoryOrder order) {
    *target = (AtomicBool)val;
}

static forceinline int32_t AtomicInt32Load(AtomicInt32* target) {
    return *(volatile int32_t*)target;
}

static forceinline bool AtomicInt32LoadByMemoryOrder(AtomicBool* target, bool val, MemoryOrder order) {
    *target = (AtomicBool)val;
}

static forceinline void AtomicInt32Store(AtomicInt32* target, int32_t val) {
    *target = (AtomicInt32)val;        // 无需通过原子指令，只需要保证state是volatile就不会被编译器优化影响，最终会在某一时刻写回内存，原子性交给CPU
}

static forceinline void* AtomicPtrLoad(volatile void* target) {
    return *(void**)target;
}

static forceinline void AtomicPtrStore(volatile void* target, void* val) {
    *(volatile void**)target = (void*)val;
}


/*
* 原子读并写
* 读并写的原子操作默认提供Full屏障
*/

/*
* 原子自增
*/
static forceinline int32_t AtomicInt32Increment(AtomicInt32* target) {
    return (int32_t)InterlockedIncrement((volatile LONG*)target);
}

static forceinline int64_t AtomicInt64Increment(AtomicInt64* target) {
    return (int64_t)InterlockedIncrement64((volatile LONG64*)target);
}

/*
* 原子自减
*/
static forceinline int32_t AtomicInt32Decrement(AtomicInt32* target) {
    return (int32_t)InterlockedDecrement((volatile LONG*)target);
}

static forceinline int64_t AtomicInt64Decrement(AtomicInt64* target) {
    return (int64_t)InterlockedDecrement64((volatile LONG64*)target);
}
/*
* 原子交换
* 修改target为value
* 返回target旧值
*/

static forceinline bool AtomicBoolExchange(AtomicBool* target, bool value) {
    return InterlockedExchange((volatile LONG*)target, (LONG)value) ? true : false;
}

static forceinline int32_t AtomicInt32Exchange(AtomicInt32* target, int32_t value) {
    return (int32_t)InterlockedExchange((volatile LONG*)target, (LONG)value);
}

static forceinline int64_t AtomicInt64Exchange(AtomicInt64* target, int64_t value) {
    return (int64_t)InterlockedExchange64((volatile LONG64*)target, (LONG64)value);
}

static forceinline void* AtomicPtrExchange(volatile void* target, void* value) {
#if defined(_WIN64)
    return (void*)AtomicInt64Exchange((AtomicInt64*)target, (int64_t)value);
#else
    return (void*)AtomicInt32Exchange((AtomicInt32*)target, (int32_t)value);
#endif
}

/*
* 原子比较并交换
* 若target == comparand，则修改target为exchange，否则不做处理
* 返回修改是否成功
*/
static forceinline bool AtomicInt32CompareExchange(AtomicInt32* target, int32_t exchange, int32_t comparand) {
    return InterlockedCompareExchange((volatile LONG*)target, (LONG)exchange, (LONG)comparand) == (LONG)comparand;
}

static forceinline bool AtomicInt64CompareExchange(AtomicInt64* target, int64_t exchange, int64_t comparand) {
    return InterlockedCompareExchange64((volatile LONG64*)target, (LONG64)exchange, (LONG64)comparand) == (LONG64)comparand;
}

static forceinline bool AtomicPtrCompareExchange(volatile void* target, void* exchange, void* comparand) {
#if defined(_WIN64)
    return AtomicInt64CompareExchange((AtomicInt64*)target, (int64_t)exchange, (int64_t)comparand);
#else
    return AtomicInt32CompareExchange((AtomicInt32*)target, (int32_t)exchange, (int32_t)comparand);
#endif
}


#endif


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONCURRENCY_ATOMIC_H_