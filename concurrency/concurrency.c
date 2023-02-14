#include <CUtils/concurrency/concurrency.h>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>

/*
* 原子自增
*/
inline int32_t AtomicIncrement32(volatile int32_t* target) {
	return InterlockedIncrement(target);
}

/*
* 原子自减
*/
inline int32_t AtomicDecrement32(volatile int32_t* target) {
	return InterlockedDecrement(target);
}

/*
* 原子交换
* 修改target为value
* 返回target旧值
*/
inline int32_t AtomicExchange32(volatile int32_t* target, int32_t value) {
	return InterlockedExchange(target, value);
}

/*
* 原子比较并交换
* 若target == comparand，则修改target为exchange
* 返回target旧值
*/
inline int32_t AtomicCompareExchange32(volatile int32_t* target, int32_t exchange, int32_t comparand) {
	return InterlockedCompareExchange(target, exchange, comparand);
}

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

#endif // Windows


void SpinLockInit(SpinLock* lock) {
	lock->state = false;
}

void SpinLockAcquire(SpinLock* lock) {
	while (AtomicExchange32(&lock->state, true) == true) { Pause(); continue; }
}

void SpinLockRelease(SpinLock* lock) {
	lock->state = false;		// AtomicExchange32(&lock->state, false);
}


void SleepLockInit(SleepLock* lock) {
	lock->state = false;
}

void SleepLockAcquire(SleepLock* lock) {
	while (AtomicExchange32(&lock->state, true) == true) { Switch(0); continue; }
}

void SleepLockRelease(SleepLock* lock) {
	lock->state = false;
}


void SleepRwLockInit(SleepRwLock* lock) {
	lock->read_count = 0;
	SleepLockInit(&lock->write_lock);
	lock->write_wait = false;
}

void SleepRwLockReadAcquire(SleepRwLock* lock) {
	while (lock->write_wait) { Pause(); continue; }      // 防止写饥饿，一旦有写开始等待读完成，后来的读就不再参与抢占
	SleepLockAcquire(&lock->write_lock);		// 先抢占写锁
	// 随后增加读计数并释放写锁
	AtomicIncrement32(&lock->read_count);		// 抢占了写锁依旧使用原子自增是因为可能存在另一个读正好释放读锁
	SleepLockRelease(&lock->write_lock);
}

void SleepRwLockReadRelease(SleepRwLock* lock) {
	AtomicDecrement32(&lock->read_count);
}

void SleepRwLockWriteAcquire(SleepRwLock* lock) {
	SleepLockAcquire(&lock->write_lock);		// 抢占写锁
	lock->write_wait = true;
	while (lock->read_count == 0) { Pause();  continue; }		// 等待正在进行的读完成
	lock->write_wait = false;
}

void SleepRwLockWriteRelease(SleepRwLock* lock) {
	SleepLockRelease(&lock->write_lock);
}
