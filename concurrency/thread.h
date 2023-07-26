/*
* Copyright ©2022-2023 @yuyuaqwq, All Rights Reserved.
*/

#ifndef LIBYUC_CONCURRENCY_THREAD_H_
#define LIBYUC_CONCURRENCY_THREAD_H_

#include <libyuc/object.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t ThreadId

#if defined(_WIN32)
#include <Windows.h>
/*
* 空转，使CPU得
以稍加休息
*/

static forceinline void ThreadPause() {
  YieldProcessor();
}

/*
* 切换，让出CPU控制权切换线程
*/
static forceinline void ThreadSwitch() {
  Sleep(0);
}

/*
* 休眠，一定时间后再被唤醒
*/
static forceinline void ThreadSleep(int duration) {
  Sleep(duration);
}

typedef void (*ThreadEntry)(void* context);
typedef struct _ThreadContext {
  ThreadEntry entry;
  void* context;
} ThreadContext;

static DWORD WINAPI ThreadForward(LPVOID lpThreadParameter) {
  ThreadContext* thread_context_ptr = (ThreadContext*)lpThreadParameter;
  ThreadContext thread_context = *thread_context_ptr;
  ObjectRelease(thread_context_ptr);
  thread_context.entry(thread_context.context);
  return 0;
}

static forceinline void ThreadCreate(ThreadEntry entry, void* context) {
  ThreadContext* thread_context = ObjectCreate(ThreadContext);
  thread_context->context = context;
  thread_context->entry = entry;
  HANDLE thread = CreateThread(NULL, NULL, ThreadForward, thread_context, 0, NULL);
  if (thread != NULL) {
    CloseHandle(thread);
  }
}

static forceinline ThreadId ThreadGetId() {
  return (ThreadId)GetCurrentThreadId();
}

#elif defined(linux)

static forceinline ThreadId ThreadGetId() {
  return (ThreadId)pthread_self();
}

#endif

#ifdef __cplusplus
}
#endif

#endif // LIBYUC_CONCURRENCY_THREAD_H_