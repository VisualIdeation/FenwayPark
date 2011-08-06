#include <cstdlib>
#include <sstream>
#include <string>

#include <UTIL/SystemBase.h>

/*
 * getCallStack - Returns a stack trace.
 *
 * @post If supported, returns a string describing the current call stack.
 */
std::string SystemBase::getCallStack() {
	std::string ret_stack(
			"Stack trace:\n   <Call stack printing not supported>\n");

	return ret_stack;
}
