/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONCURRENCY_TLS_H_
#define LIBYUC_CONCURRENCY_TLS_H_

/*
* Thread Local Storage - 线程局部存储
*/

#include <libyuc/object.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
#define thread_local __declspec(thread)
#elif defined(__GNUC__)
#define thread_local __thread
#elif defined(__clang__)
#endif

typedef uint32_t TlsId;

#if defined(_WIN32)
#include <Windows.h>


static forceinline TlsId TlsAlloc_() {
  return (TlsId)TlsAlloc();
}

static forceinline bool TlsFree_(TlsId id) {
  return TlsFree((DWORD)id);
}

static forceinline bool TlsSetValue_(TlsId id, const void* value) {
  return TlsSetValue((DWORD)id, (LPVOID)value);
}

static forceinline void* TlsGetValue_(TlsId id) {
  TlsGetValue((DWORD)id);
}

#define TlsAlloc TlsAlloc_
#define TlsFree TlsFree_
#define TlsSetValue TlsSetValue_
#define TlsGetValue TlsGetValue_


#elif defined(linux)


#endif

#ifdef __cplusplus
}
#endif

#endif //LIBYUC_CONCURRENCY_TLS_H_