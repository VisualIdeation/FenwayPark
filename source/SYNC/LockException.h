#ifndef LOCK_EXCEPTION_H_
#define LOCK_EXCEPTION_H_

#include <UTIL/Exception.h>

/*
 * LockException - Exception type thrown if an error occurs while performing
 * an operation on a lock object.
 */
class LockException: public Exception {
public:
			LockException(const std::string& msg, const std::string& location =
					"") throw ();

	virtual ~LockException() throw ();

	virtual std::string getExceptionName() const {
		return "LockException";
	}
};

#endif /* LOCK_EXCEPTION_H_ */
