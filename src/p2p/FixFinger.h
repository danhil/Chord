#ifndef FIXFINGER_H
#define FIXFINGER_H

#include "ThreadUtil.h"
#include "ChordNode.h"
#include "DebugUtil.h"

class FixFinger: public Thread {

public:
	FixFinger(ChordNode * n)
	: Thread(Debug::GetDebugger())
	, node(n)
	, debug(Debug::GetDebugger())
	{}

	void Run()
	{
		while(true)
	 	{
			std::cout << "Fixing the fingers table..." << std::endl;
			node->fixFingersTable();
			// Sleep for int32_t ms
			this->SleepMs(node->getTimeToCheck());
		}
	}

private:
	ChordNode * node;
	std::shared_ptr<Debug> debug;
};

#endif
