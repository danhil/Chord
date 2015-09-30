#ifndef _NetworkOverlay_H
#define _NetworkOverlay_H

#include "Node.h"
#include "ITransport.h"
#include <string>
#include <stdint.h>

class NetworkOverlay
{

protected:
	virtual std::string getIdentifier(void) = 0;
	virtual ITransport * getTransport(void) = 0;
	virtual Node * getThisNode(void) = 0;
  // Get and put a key/value pair
	virtual void put(std::string key_i, std::string value_i) = 0;
	virtual std::string get(std::string key_i) = 0;
};

#endif
