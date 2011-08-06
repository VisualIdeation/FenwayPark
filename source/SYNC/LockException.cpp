#include <SYNC/LockException.h>

LockException::LockException(const std::string& msg,
		const std::string& location) throw () :
	Exception(msg, location) {
	/* Do nothing. */;
}

LockException::~LockException() throw () {
	/* Do nothing. */;
}
