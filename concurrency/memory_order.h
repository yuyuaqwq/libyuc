/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONCURRENCY_ATOMIC_H_
#define LIBYUC_CONCURRENCY_ATOMIC_H_

#include <libyuc/object.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* 内存序
* 参考自 C++11 标准
*/

enum MemoryOrder{
  kMemoryOrderRelaxed,
  kMemoryOrderConsume,
  kMemoryOrderAcquire,
  kMemoryOrderRelease,
  kMemoryOrderAcqRel,
  kMemoryOrderSeqCst,
};

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONCURRENCY_ATOMIC_H_