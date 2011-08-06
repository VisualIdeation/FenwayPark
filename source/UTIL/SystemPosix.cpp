#include <cstdlib>
#include <cstring>
#include <sys/utsname.h>

#include <UTIL/SystemPosix.h>

/*
 * usleep - Sleeps for the given number of microseconds.
 *
 * @param micro The number of microseconds to sleep.
 */
int SystemPosix::usleep(Uint32 micro) {
	return ::usleep(micro);
} // end usleep()

/*
 * msleep - Sleeps for the given number of milliseconds.
 *
 * @param milli The number of milliseconds to sleep.
 */
int SystemPosix::msleep(Uint32 milli) {
	// usleep() cannot sleep for more than 1 second, so we have to work
	// around that here.  First, we sleep for N seconds.
	if (milli >= 1000) {
		SystemPosix::sleep(milli / 1000);
	}

	// Then we finish off by sleeping for (N mod 1000) milliseconds.
	return SystemPosix::usleep((milli % 1000) * 1000);
} // end msleep()

/**
 * Ntohll - Converts the given 64-bit value (a long long) from native byte
 * ordering to network byte ordering.  This is safe to use with signed and
 * unsigned values.
 */
Uint64 SystemPosix::Ntohll(Uint64 conversion) {
	Uint64 ret_val;

	if (isLittleEndian()) {
		*(reinterpret_cast<Uint32*> (&ret_val) + 1) = Ntohl(
				*reinterpret_cast<Uint32*> (&conversion));
		*reinterpret_cast<Uint32*> (&ret_val) = Ntohl(
				*(reinterpret_cast<Uint32*> (&conversion) + 1));
	} else {
		*reinterpret_cast<Uint32*> (&ret_val) = Ntohl(
				*reinterpret_cast<Uint32*> (&conversion));
		*(reinterpret_cast<Uint32*> (&ret_val) + 1) = Ntohl(
				*(reinterpret_cast<Uint32*> (&conversion) + 1));
	}

	return ret_val;
} // end Ntohll()

/*
 * Htonll - Converts the given 64-bit value (a long long) from network byte
 * ordering to native byte ordering.  This is safe to use with signed and
 * unsigned values.
 */
Uint64 SystemPosix::Htonll(Uint64 conversion) {
	Uint64 ret_val;

	if (isLittleEndian()) {
		*(reinterpret_cast<Uint32*> (&ret_val) + 1) = Htonl(
				*reinterpret_cast<Uint32*> (&conversion));
		*reinterpret_cast<Uint32*> (&ret_val) = Htonl(
				*(reinterpret_cast<Uint32*> (&conversion) + 1));
	} else {
		*reinterpret_cast<Uint32*> (&ret_val) = Htonl(
				*reinterpret_cast<Uint32*> (&conversion));
		*(reinterpret_cast<Uint32*> (&ret_val) + 1) = Htonl(
				*(reinterpret_cast<Uint32*> (&conversion) + 1));
	}

	return ret_val;
} // end Htonll()

/*
 * getenv - Queries the run-time environment for the value of the named
 * environment variable.
 *
 * @param name   The name of the environment variable to query.
 * @param result Storage for the value of the named environment variable.
 *
 * @return \c true is returned if the named environment variable is defined
 *         in the run-time environment and lookup of the variable succeeded.
 *         \c false is returned if the named environment variable could not
 *         be found in the run-time environment.
 */
bool SystemPosix::getenv(const std::string& name, std::string& result) {
	char* val;
	bool status(false);

	val = std::getenv(name.c_str());

	if (val != NULL) {
		result = val;
		status = true;
	}

	return status;
} // end getenv()

/*
 * setenv - Sets the value of the named environment variable in the run-time
 * environment.  If \p value is "", then it either unsets the variable or
 * clears it.
 *
 * @post On successful completion, the named environment variable has the
 *       given value in the run-time environment.  If the named environment
 *       variable does not exist in the run-time environment, it will be
 *       created.
 *
 * @param name  The name of the environment variable to set.
 * @param value The value to assign to the named environment variable.
 *
 * @return \c true is returned if the named environment was set to the new
 *         value successfully. \c false is returned if the environment
 *         variable set operation failed.
 */
bool SystemPosix::setenv(const std::string& name, const std::string& value) {
	std::string set_value(name);
	set_value += "=";
	set_value += value;

	// Purposely leak memory since putenv(3) may want to hold on to the
	// pointer we pass.
	char* env_str = strdup(set_value.c_str());
	const int ret_val = ::putenv(env_str);

	return ret_val == 0;
} // end setenv()

/*
 * getHostname - Returns the name of the local host.
 */
std::string SystemPosix::getHostname() {
	struct utsname buffer;

	if (uname(&buffer) == 0) {
		char* temp;
		temp = std::strchr(buffer.nodename, '.');

		// If the node name contains the full host, dots and all, truncate it
		// at the first dot.
		if (temp != NULL) {
			*temp = '\0';
		}

		return std::string(buffer.nodename);
	} else {
		return std::string("<hostname-lookup failed>");
	}
} // end getHostname()
