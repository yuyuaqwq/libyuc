/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONCURRENCY_MEMORY_BARRIER_H_
#define LIBYUC_CONCURRENCY_MEMORY_BARRIER_H_

#include <libyuc/basic.h>

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
* 读屏障
* 在读语句前插入读屏障，可以使当前高速缓存的数据失效，从主存中重新加载
*/
static forceinline void MemoryBarrierLoad() {
  _mm_lfence();
}

/*
* 写屏障
* 在写语句后插入写屏障，能使当前写入到高速缓存中的数据写回主存，于当前核而言保证写语句对写屏障后的读语句可见(不产生重排)(但其他核可能并未重读缓存)
*/
static forceinline void MemoryBarrierStore() {
  _mm_sfence();
}

/*
* 通用屏障(Full)
* 严格保证全屏障前的读写语句对全屏障后的读写语句完全可见(执行全屏障后于其他核也是可见的)
*/
static forceinline void MemoryBarrierFull() {
  // __faststorefence();
  _mm_mfence();
}

#endif


#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONCURRENCY_MEMORY_BARRIER_H_