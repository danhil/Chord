#ifndef __Exception__
#define __Exception__

#include <iostream>
#include <exception>
#include <string>

class Exception: private std::exception {

private:

  std::string error;

  const std::string & getHeading(void)
  {
    static const std::string headingString("Exception Raised : ");
    return headingString;
  }

public:

	Exception() : exception()
	{
		this->error = "Uncaught exception";
	}

	Exception(std::string e) : exception()
	{
		this->error = e;
	}

	~Exception() throw ()	{}

	const char* what(void)
	{
		return (this->error.c_str());
	}

	const char * toString(void)
	{
		return (this->error.c_str());
	}

	void printStackTrace(void)
	{

		std::cout << getHeading() << this->error << std::endl;
	}

};

#endif
