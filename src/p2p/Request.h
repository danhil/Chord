#ifndef _request_h
#define _request_h

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <assert.h>

class Debug;

class Request
{

public:

  Request(const std::string & overlayID_i, const int32_t & code_i);

	int32_t getCode(void) const
	{
		return code_m;
	}

  std::string getOverlayID(void) const
	{
		return overlayID_m;
	}

	void setCode(const int32_t & code_i)
	{
		this->code_m = code_i;
	}

	void setOverlayID(const std::string & overlayID_i)
	{
		this->overlayID_m = overlayID_i;
	}

	void addArg(const std::string &key_i, const std::string &value_i);

  std::string getArg(const std::string &key_i);

  std::string serialize(void);

	uint32_t getCheckSum(const std::string &str_i);

private:
	typedef std::pair<std::string, std::string> arg;
	typedef std::map<std::string, std::string> argMap;
  // The overlay id for the reqeust
  std::string overlayID_m;
  // The transport code for the request
	int32_t code_m;
  // Map of string, string arguments
	argMap arguments;
  std::shared_ptr<Debug> debug;
};

#endif
