#include <CUtils/concurrency/rw_lock.h>

void RwLockInit(RwLock* lock) {
	lock->read_count = 0;
	MutexLockInit(&lock->write_lock);
	lock->write_wait = false;
}

void RwLockReadAcquire(RwLock* lock) {
	while (lock->write_wait) { Pause(); continue; }      // 防止写饥饿，一旦有写开始等待读完成，后来的读就不再参与抢占
	MutexLockAcquire(&lock->write_lock);		// 先抢占写锁
	// 随后增加读计数并释放写锁
	AtomicIncrement32(&lock->read_count);		// 抢占了写锁依旧使用原子自增是因为可能存在另一个读正好释放读锁
	MutexLockRelease(&lock->write_lock);
}

void RwLockReadRelease(RwLock* lock) {
	AtomicDecrement32(&lock->read_count);
}

void RwLockWriteAcquire(RwLock* lock) {
	MutexLockAcquire(&lock->write_lock);		// 抢占写锁
	lock->write_wait = true;
	while (lock->read_count == 0) { Pause();  continue; }		// 等待正在进行的读完成
	lock->write_wait = false;
}

void RwLockWriteRelease(RwLock* lock) {
	MutexLockRelease(&lock->write_lock);
}
