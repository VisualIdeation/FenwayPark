#ifndef RESOURCE_EXCEPTION_H_
#define RESOURCE_EXCEPTION_H_

#include <UTIL/Exception.h>

/*
 * ResourceException - Exception type thrown if resource allocation fails.
 */
class ResourceException: public Exception {
public:
			ResourceException(const std::string& msg,
					const std::string& location = "") throw ();

	virtual ~ResourceException(void) throw ();

	virtual std::string getExceptionName(void) const {
		return ":ResourceException";
	}
};

#endif /* RESOURCE_EXCEPTION_H_ */
