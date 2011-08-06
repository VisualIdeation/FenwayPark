
#ifndef DEADLOCK_EXCEPTION_H_
#define DEADLOCK_EXCEPTION_H_

#include <SYNC/LockException.h>

/*
 * DeadlockException - Exception type thrown if deadlock would occur when
 * trying to perform an operation on a lock object.
 */
class DeadlockException : public LockException
{
public:
   DeadlockException(const std::string& msg, const std::string& location = "")
      throw ();

   virtual ~DeadlockException() throw ();

   virtual std::string getExceptionName() const
   {
      return "DeadlockException";
   }
};

#endif /* DEADLOCK_EXCEPTION_H_ */
