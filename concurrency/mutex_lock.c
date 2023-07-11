#include <libyuc/concurrency/mutex_lock.h>
#include <libyuc/concurrency/thread.h>
#include <libyuc/concurrency/atomic.h>

void MutexLockInit(MutexLock* lock) {
	lock->state = false;
}

void MutexLockAcquire(MutexLock* lock) {
	while (AtomicExchange32(&lock->state, true) == true) { ThreadSwitch(0); continue; }
}

void MutexLockRelease(MutexLock* lock) {
	lock->state = false;
}
