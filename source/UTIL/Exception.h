#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>
#include <stdexcept>

/* Boost */
#include <boost/preprocessor/stringize.hpp>

#define LOCATION std::string(__FILE__) + std::string(":") + \
           std::string(BOOST_PP_STRINGIZE(__LINE__))

/**
 * @example "Example of using exceptions"
 *
 * All exceptions derive from Exception, and its constructor takes two
 * parameters: a description of the error and an optional string describing the
 * location in the code at which the error occurred. The easiest way to get the
 * location information is to use the preprocessor symbol \c LOCATION.
 *
 * \code
 * throw Exception("An error occurred", LOCATION);
 * \endcode
 */

/*
 * Exception - Base exception for all exceptions. Exception areas:
 * - I/O loading/saving issues
 * - Property access errors
 * - Invalid data type errors
 */
class Exception: public std::runtime_error {
public:
	Exception(const std::string& desc, const std::string& location) throw ();
	virtual ~Exception() throw ();

	virtual const char* what() const throw ();

	virtual std::string getExceptionName() const;

	const std::string& getDescription() const;
	void setDescription(const std::string& desc);

	const std::string& getLocation() const;
	const std::string& getStackTrace() const;

	virtual std::string getExtendedDescription() const;

	virtual std::string getFullDescription() const;

protected:
	std::string description;
	std::string location;
	std::string stackTrace;

	mutable std::string fullDescription;
};

#endif /* EXCEPTION_H_ */
