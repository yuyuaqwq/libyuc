#include <CUtils/concurrency/mutex_lock.h>
#include <CUtils/concurrency/thread.h>
#include <CUtils/concurrency/atomic.h>

void MutexLockInit(MutexLock* lock) {
	lock->state = false;
}

void MutexLockAcquire(MutexLock* lock) {
	while (AtomicExchange32(&lock->state, true) == true) { ThreadSwitch(0); continue; }
}

void MutexLockRelease(MutexLock* lock) {
	lock->state = false;
}
