#include <cstring>
#include <sstream>

#include <SYNC/MutexPosix.h>
#include <UTIL/ResourceException.h>

/**
 * MutexPosix - Constructor for MutexPosix class.
 *
 * @post The mutex variable is initialized and ready for use. The mutex
 *       operations will not fail.
 *
 * @throw ResourceException is thrown if the mutex cannot be allocated.
 */
MutexPosix::MutexPosix(void) {
	int result(0);

	// Initialize the mutex.
#ifndef DEBUG
	result = pthread_mutex_init(&mutex, NULL);
#else
	pthread_mutexattr_t mutex_attr;
	pthread_mutexattr_init(&mutex_attr);
	pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK_NP);
	result = pthread_mutex_init(&mutex, &mutex_attr);
	pthread_mutexattr_destroy(&mutex_attr);
#endif
	if (result != 0) {
		std::ostringstream msg_stream;
		msg_stream << "Mutex allocation failed: " << std::strerror(result);
		throw ResourceException(msg_stream.str(), LOCATION);
	}
} // end MutexPosix()

/**
 * test - Tests the current lock status.
 *
 * @post The state of the mutex variable is returned.
 *
 * @return \c false is returned if this mutex is not currently locked.
 *         \c true is returned if it is.
 */
bool MutexPosix::test(void) const {
	bool locked(true);

	const int status = pthread_mutex_trylock(
			const_cast<pthread_mutex_t*> (&mutex));

	// If the return value from pthread_mutex_trylock(3) is 0, then this thread
	// now has a lock on mutex. Therefore, no other thread could have held the
	// mutex, so unlock the mutex and return false.
	if (status == 0) {
		pthread_mutex_unlock(const_cast<pthread_mutex_t*> (&mutex));
		locked = false;
	}

	return locked;
} // end test()
