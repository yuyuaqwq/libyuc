/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONCURRENCY_TLS_H_
#ifndef LIBYUC_CONCURRENCY_TLS_H_

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

static forceinline void TlsFree_(TlsId id) {
  return TlsFree((DWORD)id);
}

static forceinline void TlsSetValue_(TlsId id, const void* value) {
  TlsSetValue((DWORD)id, value);
}

static forceinline void* TlsGetValue_(TlsId id) {
  TlsGetValue((DWORD)id);
}

#define TlsAlloc TlsAlloc_
#define TlsFree TlsFree_
#define TlsSetValue TlsSetValue_
#define TlsGetValue TlsGetValue_


#elif defined(linux)

static forceinline TlsId TlsAlloc() {
  return (TlsId)pthread_key_create();
}

static forceinline void TlsFree(TlsId id) {
  return pthread_key_delete((pthread_key_t)id);
}

static forceinline void TlsSetValue(TlsId id, const void* value) {
  TlsSetValue((pthread_key_t)id, value);
}

static forceinline void* TlsGetValue(TlsId id) {
  TlsGetValue((pthread_key_t)id);
}

#endif

#ifdef __cplusplus
}
#endif

#ifndef LIBYUC_CONCURRENCY_TLS_H_