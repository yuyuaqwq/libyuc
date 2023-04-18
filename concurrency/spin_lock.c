#include <CUtils/concurrency/spin_lock.h>
#include <CUtils/concurrency/atomic.h>

void SpinLockInit(SpinLock* lock) {
	lock->state = false;
}

void SpinLockAcquire(SpinLock* lock) {
	while (AtomicExchange32(&lock->state, true) == true) { Pause(); continue; }
}

void SpinLockRelease(SpinLock* lock) {
	lock->state = false;		// AtomicExchange32(&lock->state, false);
}