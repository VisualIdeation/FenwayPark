/**
 * SystemPosix
 *
 * @note This file MUST be included by UTIL/System.h, not the other way around.
 */

#ifndef SYSTEM_POSIX_H_
#define SYSTEM_POSIX_H_

#include <string>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/param.h>

#include <UTIL/SystemBase.h>
#include <UTIL/Types.h>

typedef struct timeval TimeVal;
typedef struct timezone TimeZone;

/*
 * SystemPosix - Low-level operating system feature abstractions using POSIX
 * functionality. This is typedef'd to System.
 */
class SystemPosix: public SystemBase {
public:
	static int usleep(Uint32 micro);
	static int msleep(Uint32 milli);

	/*
	 * sleep - Sleeps for the given number of seconds.
	 *
	 * @param seconds The number of seconds to sleep.
	 */
	static int sleep(Uint32 seconds) {
		return ::sleep(seconds);
	} // end sleep()

	/**
	 * gettimeofday - Gets the current time of day in seconds since January 1, 1970.
	 *
	 * @param tp  Storage for the timeval data structure.
	 * @param tzp Storage for the time zone.  This parameter is optional and
	 *            defaults to NULL.
	 */
	static int gettimeofday(TimeVal* tp, TimeZone* tzp = NULL) {
		return ::gettimeofday(tp, tzp);
	} // end gettimeofday()

	/*
	 * Ntohs - Converts the given 16-bit value (a short) from native byte
	 * ordering to network byte ordering.  This is safe to use with signed and
	 * unsigned values.
	 */
	static Uint16 Ntohs(Uint32 conversion) {
		return ntohs(conversion);
	} // end Ntohs()

	/**
	 * Ntohl - Converts the given 32-bit value (a long) from native byte
	 * ordering to network byte ordering.  This is safe to use with signed and
	 * unsigned values.
	 */
	static Uint32 Ntohl(Uint32 conversion) {
		return ntohl(conversion);
	} // end Ntohl()

	static Uint64 Ntohll(Uint64 conversion);

	/*
	 * Htons - Converts the given 16-bit value (a short) from network byte
	 * ordering to native byte ordering.  This is safe to use with signed and
	 * unsigned values.
	 */
	static Uint16 Htons(Uint16 conversion) {
		return htons(conversion);
	} // end Htons()

	/*
	 * Htonl - Converts the given 32-bit value (a long) from network byte
	 * ordering to native byte ordering.  This is safe to use with signed and
	 * unsigned values.
	 */
	static Uint32 Htonl(Uint32 conversion) {
		return htonl(conversion);
	} // end Htonl()

	static Uint64 Htonll(Uint64 conversion);
	static bool getenv(const std::string& name, std::string& result);
	static bool setenv(const std::string& name, const std::string& value);
	static std::string getHostname();
};

#endif /* SYSTEM_POSIX_H_ */
