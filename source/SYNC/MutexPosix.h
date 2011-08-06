/*
 * MutexPosix
 *
 * @note This file must be included by SYNC/Mutex.h, not the other way around.
 */

#ifndef MUTEX_POSIX_H_
#define MUTEX_POSIX_H_

#include <cstdio>
#include <pthread.h>
#include <errno.h>
#include <assert.h>

/* Boost includes */
#include <boost/noncopyable.hpp>
#include <boost/concept_check.hpp>

#include <SYNC/LockException.h>
#include <SYNC/DeadlockException.h>

/*
 * MutexPosix - Mutex wrapper for POSIX-compliant systems using pthreads mutex
 * variables for the implementation. This is typedef'd to Mutex.
 */
class MutexPosix: boost::noncopyable {
public:
	MutexPosix(void);
	/**
	 * ~MutexPosix - destructor for MutexPosix class.
	 *
	 * @pre No thread should be in a lock-specific function.
	 * @post The mutex variable is destroyed.
	 */
	~MutexPosix(void) {
		const int result = pthread_mutex_destroy(&mutex);
		assert(result == 0);
		boost::ignore_unused_variable_warning(result);
	} // end ~MutexPosix()

	/*
	 * acquire - Locks this mutex.
	 *
	 * @post A lock on the mutex variable is acquired by the caller.  If a
	 *       lock has already been acquired by another thread, the caller
	 *       blocks until the mutex has been freed.
	 *
	 * @throw DeaadockException is thrown if the current thread has
	 *        already locked this mutex.
	 */
	void acquire(void) {
		const int result = pthread_mutex_lock(&mutex);

		/*
		 * This thread tried to lock the mutex twice, and a deadlock condition
		 * was reported.
		 */
		if (EDEADLK == result) {
			throw DeadlockException(
					"Tried to lock mutex twice in the same thread", LOCATION
			);
		} else if (EINVAL == result) {
			throw LockException(
					"Tried to lock a mutex that does not refer to a initialized mutex object.",
					LOCATION
			);
		} else if (EAGAIN == result) {
			throw LockException(
					"Tried to lock a mutex whose maximum number of recursive locks has been exceeded.",
					LOCATION
			);
		}
		assert(result == 0);
		boost::ignore_unused_variable_warning(result);
	} // end acquire()

	/*
	 * acquireRead - Acquires a read lock on this mutex.
	 *
	 * @post A lock on the mutex variable is acquired by the caller.  If a
	 *       lock has already been acquired by another thread, the caller
	 *       blocks until the mutex has been freed.
	 *
	 * @note No special read lock has been defined for now.
	 */
	void acquireRead(void) {
		this->acquire();
	} // end acquireRead()

	/*
	 * acquireWrite - Acquires a write lock on this mutex.
	 *
	 * @post A lock on the mutex variable is acquired by the caller.  If a
	 *       lock has already been acquired by another thread, the caller
	 *       blocks until the mutex has been freed.
	 *
	 * @note No special write lock has been defined for now.
	 */
	void acquireWrite(void)
	{
		this->acquire();
	} // end acquireWrite()

	/*
	 * tryAcquire - Tries to acquire a lock on this mutex variable (does not block).
	 *
	 * @post A lock on the mutex variable is acquired by the caller.  If a
	 *       lock has already been acquired by another thread, the caller
	 *       returns does not wait for it to be unlocked.
	 *
	 * @return \c true is returned if the lock is acquired, and \c false is
	 *         returned if the mutex is already locked.
	 */
	bool tryAcquire(void) {
		return pthread_mutex_trylock(&mutex) == 0;
	} // end tryAcquire()

	/*
	 * tryAcquireRead - Tries to acquire a read lock on this mutex (does not block).
	 *
	 * @post A lock on the mutex variable is acquired by the caller.  If a
	 *       lock has already been acquired by another thread, the caller
	 *       returns does not wait for it to be unlocked.
	 *
	 * @return \c true is returned if the lock is acquired, and \c false is
	 *         returned if the mutex is already locked.
	 */
	bool tryAcquireRead(void) {
		return this->tryAcquire();
	} // end tryAcquireRead()

	/*
	 * tryAcquireWrite - Tries to acquire a write lock on this mutex (does not block).
	 *
	 * @post A lock on the mutex variable is acquired by the caller.  If a
	 *       lock has already been acquired by another thread, the caller
	 *       returns does not wait for it to be unlocked.
	 *
	 * @return \c true is returned if the lock is acquired, and \c false is
	 *         returned if the mutex is already locked.
	 */
	bool tryAcquireWrite(void) {
		return this->tryAcquire();
	} // end tryAcquireWrite()

	/*
	 * release - Releases this mutex.
	 *
	 * @pre The mutex variable must be locked.
	 * @post The mutex variable is unlocked.
	 *
	 * @throw LockException is thrown if the current thread was not the
	 *        one that locked this mutex.
	 */
	void release(void) {
		const int result = pthread_mutex_unlock(&mutex);

		if (EPERM == result) {
			throw LockException(
					"Tried to release a mutex that this thread does not own",
					LOCATION
			);
		}

		assert(result == 0);
		boost::ignore_unused_variable_warning(result);
	} // end release()

	bool test(void) const;

	// Allow the CondPosix class to access the private and protected
	// members of this class.  Specifically, direct access is needed to the
	// mutex variable.
	friend class CondVarPosix;

protected:
	pthread_mutex_t mutex;
};

#endif  /* MUTEX_POSIX_H_ */
