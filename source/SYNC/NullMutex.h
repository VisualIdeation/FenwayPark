#ifndef NULL_MUTEX_H_
#define NULL_MUTEX_H_

#include <cstdio>

/* Boost includes */
#include <boost/noncopyable.hpp>

/*
 * NullMutex - Null mutex wrapper. Used to pass a do nothing mutex as a template type.
 */
class NullMutex: boost::noncopyable {
public:
	NullMutex(void) {
		;
	}

	~NullMutex(void) {
		;
	}

	/*
	 * acquire - Locks the mutex.
	 */
	void acquire(void) {
		/* Do nothing. */;
	} // end acquire()

	/*
	 * acquireRead - Acquires a read mutex.
	 */
	void acquireRead(void) {
		this->acquire();
	} // end acquireRead()

	/*
	 * acquireWrite - Acquires a write mutex.
	 */
	void acquireWrite(void) {
		this->acquire();
	} // end acquireWrite()

	/*
	 * tryAcquire - Tries to acquire the lock.  Returns immediately even if we
	 * don't acquire the lock.
	 *
	 * @return \c true is always returned.
	 */
	bool tryAcquire(void) {
		return true;
	} // end tryAcquire()

	/*
	 * tryAcquireRead - Tries to acquire a read mutex.
	 */
	bool tryAcquireRead(void) {
		return this->tryAcquire();
	} // end tryAcquireRead()

	/*
	 * tryAcquireWrite - Tries to acquire a write mutex.
	 */
	bool tryAcquireWrite(void) {
		return this->tryAcquire();
	} // end tryAcquireWrite()

	/*
	 * release - Releases the mutex.
	 */
	void release(void) {
		/* Do nothing. */;
	} // end release()

	/*
	 * test - Tests the current lock status.
	 *
	 * @return \c false is always returned.
	 */
	bool test(void) const {
		return false; // Just return false since it is a null lock
	} // end test()

	/*
	 * dump - Dumps the mutex debug stuff and current state.
	 */
	void dump(FILE* dest = stderr, const char* message =
			"\n------ Mutex Dump -----\n") const {
		fprintf(dest, "%sNULL Mutex", message);
	} // end dump()
};

#endif /* NULL_MUTEX_H_ */
