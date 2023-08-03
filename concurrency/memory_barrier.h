/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONCURRENCY_MEMORY_BARRIER_H_
#define LIBYUC_CONCURRENCY_MEMORY_BARRIER_H_

#include <libyuc/object.h>

/*
* 内存屏障
* 解决指令重排、缓存一致性等问题
*/


/*
* Store
* 使当前写入到高速缓存中的数据写回主存
* Load
* 使当前高速缓存的数据失效，从主存中重新加载
*/


#if defined(_WIN32)
#include <Windows.h>
#include <intrin.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
* 读读屏障
* 在读语句前插入读读屏障，可以使当前高速缓存的数据失效，从主存中重新加载
* Load1; LoadLoad; Load2
* 该屏障确保Load1数据的装载先于Load2及其后所有装载指令的的操作
*/
static forceinline void MemoryBarrierLoadLoad() {
  _mm_lfence();
}

/*
* 写写屏障
* 在写语句后插入写写屏障，能使当前写入到高速缓存中的数据写回主存，于当前核而言保证写语句对写写屏障后的读语句可见(不产生重排)(但其他核可能并未重读缓存)
* Store1; StoreStore; Store2
* 该屏障确保Store1立刻刷新数据到内存(使其对其他处理器可见)的操作先于Store2及其后所有存储指令的操作
*/
static forceinline void MemoryBarrierStoreStore() {
  _mm_sfence();
}

/*
* 读写屏障
* Load1; LoadStore; Store2
* 确保Load1的数据装载先于Store2及其后所有的存储指令刷新数据到内存的操作
*/
static forceinline void MemoryBarrierLoadStore() {
  _mm_mfence();
}

/*
* 写读屏障(Full)
* 严格保证全屏障前的读写语句对全屏障后的读写语句完全可见(执行全屏障后于其他核也是可见的)
* Store1;StoreLoad;Load2
* 该屏障确保Store1立刻刷新数据到内存的操作先于Load2及其后所有装载装载指令的操作。它会使该屏障之前的所有内存访问指令(存储指令和访问指令)完成之后,才执行该屏障之后的内存访问指令
*/
static forceinline void MemoryBarrierStoreLoad() {
  // __faststorefence();
  _mm_mfence();
}

#endif


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONCURRENCY_MEMORY_BARRIER_H_