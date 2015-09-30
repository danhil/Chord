#ifndef ITRANSPORT_H
#define ITRANSPORT_H

#include <string>
#include "Node.h"
#include "Request.h"

class ITransport
{

public:

  ITransport() : port_m(-1){}

	virtual char * sendRequest(Request * request, Node * destination) = 0;

	virtual	std::string	connectToTracker(std::string & ip, int32_t port, Node * n) = 0;

	const int32_t & getPort(void)
	{
	  return port_m;
	}

	void setPort(const int32_t port_i )
	{
	  port_m = port_i;
	}

protected:
	int32_t port_m;
};

#endif
