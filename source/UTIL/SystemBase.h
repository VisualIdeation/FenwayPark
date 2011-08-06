#ifndef SYSTEM_BASE_H_
#define SYSTEM_BASE_H_

#include <string>
#include <sys/time.h>

/*
 * SystemBase - SystemBase is a base class for System.  For cross-platform system
 * functions, use System.  Never use this class directly.
 *
 * @see SystemPosix
 */
class SystemBase {
public:
	/**
	 * getEndian - Determines the endianness of the host platform.  A return
	 * value of 0 means that the host uses little-endian byte order.  A return
	 * value of 1 means that the host uses big-endian byte order.
	 *
	 * @return 0 is returned for little-endian hosts.
	 * @return 1 is returned for big-endian hosts.
	 */
	static int getEndian(void) {
		union {
			char c[sizeof(short)];
			short value;
		} endian;

		// The way this works is that we access the first byte of endian.value
		// directly.  If it is 1, the host treats that as the high-order byte.
		// Otherwise, it is the low-order byte.
		endian.value = 256;

		return endian.c[0];
	} // end getEndian()

	/*
	 * isLittleEndian - Tells if the host uses little-endian byte order or not.
	 *
	 * @return \c true is returned on a little-endian host.
	 * @return \c false is returned on a big-endian host.
	 */
	static bool isLittleEndian() {
		return (getEndian() == 0);
	} // end isLittleEndian()

	/*
	 * isBigEndian - Tells if the host uses big-endian byte order or not.
	 *
	 * @return \c true is returned on a big-endian host.
	 * @return \c false is returned on a little-endian host.
	 */
	static bool isBigEndian() {
		return (getEndian() == 1);
	} // end isBigEndian()

	static std::string getCallStack();
};

#endif /* SYSTEM_BASE_H_ */
