#include <SYNC/DeadlockException.h>

DeadlockException::DeadlockException(const std::string& msg,
		const std::string& location) throw () :
	LockException(msg, location) {
	/* Do nothing. */;
}

DeadlockException::~DeadlockException() throw () {
	/* Do nothing. */;
}
