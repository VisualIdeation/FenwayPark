
#include <iostream>

#include <UTIL/Exception.h>
#include <UTIL/System.h>

Exception::Exception(const std::string& _description, const std::string& _location)
   throw()
   : std::runtime_error(_description)
   , description(_description)
   , location(_location)
{
   stackTrace = SystemPosix::getCallStack();
}

Exception::~Exception() throw()
{
   /* Do nothing. */ ;
}

const char* Exception::what() const throw()
{
   fullDescription = getFullDescription();
   return fullDescription.c_str();
}

std::string Exception::getExceptionName() const
{
   return std::string("Exception");
}

const std::string& Exception::getDescription() const
{
   return description;
}

void Exception::setDescription(const std::string& desc)
{
   description = desc;
}

const std::string& Exception::getLocation() const
{
   return location;
}

const std::string& Exception::getStackTrace() const
{
   return stackTrace;
}

std::string Exception::getExtendedDescription() const
{
   return this->getExceptionName() + std::string(": ") + getDescription();
}

std::string Exception::getFullDescription() const
{
   return getExtendedDescription() + std::string("  ") + location +
             std::string("\n") + stackTrace;
}
