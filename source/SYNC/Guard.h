#ifndef GUARD_H_
#define GUARD_H_

/*
 * Guard - Scoped wrapper for a lock.
 *
 * @date June 6, 2010
 */
template<class LOCK_TYPE>
class Guard {
public:
	/**
	 * Guard - Acquires the lock implicitly. If \p block is true, then use a
	 * blocking mutex acquisition operation. Otherwise, use a non-blocking
	 * acquisition call.
	 *
	 * @post \c lockStatus reflects whether the given lock was acquired.
	 *
	 * @param lock  The mutex to associate with this guard.
	 * @param block A flag indicating whether a blocking acquisition operation
	 *              should be used to acquire the lock. This parameter is
	 *              optional and defaults to true if it is not specified.
	 */
	Guard(LOCK_TYPE& lock, const bool block = true) :
		theLock(&lock) {
		lockStatus = block ? acquire() : tryAcquire();
	} // end Guard()

	/*
	 * ~Guard - Releases the lock.
	 */
	~Guard(void) {
		if (lockStatus) {
			theLock->release();
		}
	} // end ~Guard()

	/**
	 * locked - Indicates whether this guard is currently locked.
	 *
	 * @return \c true is returned if this guard is locked; \c false is
	 *         returned otherwise.
	 */
	bool locked(void) const {
		return lockStatus;
	} // end locked()

	/*
	 * acquire - Acquires the lock.
	 */
	bool acquire(void) {
		theLock->acquire();
		lockStatus = true;
		return lockStatus;
	} // end acquire()

	/*
	 * tryAcquire - Tries to acquire lock.
	 */
	bool tryAcquire(void) {
		lockStatus = theLock->tryAcquire();
		return lockStatus;
	} // end tryAcquire()

	/*
	 * release - Explicity releases the lock.
	 */
	void release(void) {
		lockStatus = false;
		theLock->release();
	} // end release()

private:
	LOCK_TYPE* theLock; /**< The lock that we are using */
	bool lockStatus; /**< Are we locked or not */
};

#endif /* GUARD_H_ */
