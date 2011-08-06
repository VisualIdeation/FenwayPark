#include <UTIL/ResourceException.h>

/****************************************************
 Constructors and Destructors of class NPP:
 ****************************************************/
/*
 * ResourceException constructor
 */
ResourceException::ResourceException(const std::string& msg,
		const std::string& location) throw () :
	Exception(msg, location) {
	/* Do nothing. */;
} // end ResourceException()

/*
 * ~ResourceException destructor
 */
ResourceException::~ResourceException(void) throw () {
	/* Do nothing. */;
} // end ~ResourceException()
