/*
* @yuyuaqwq - 鱼鱼
* email:1454832774@qq.com
* project:https://github.com/yuyuaqwq/CUtils
* 请保留此声明
*/

#ifndef CUTILS_CONCURRENCY_THREAD_H_
#define CUTILS_CONCURRENCY_THREAD_H_

#include <CUtils/object.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
/*
* 空转，使CPU得以稍加休息
*/
static inline void Pause() {
	YieldProcessor();
}

/*
* 切换，让出CPU控制权切换线程
*/
static inline void Switch() {
	Sleep(0);
}

/*
* 休眠，一定时间后再被唤醒
*/
static inline void Dormancy(int duration) {
	Sleep(duration);
}

#endif

#ifdef __cplusplus
}
#endif

#endif // CUTILS_CONCURRENCY_THREAD_H_