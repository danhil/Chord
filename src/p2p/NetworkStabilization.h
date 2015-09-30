#ifndef STABILIZATION_H
#define STABILIZATION_H

#include "ThreadUtil.h"
#include "DebugUtil.h"
#include "ChordNode.h"
#include "SingletonChord.h"
#include "DebugUtil.h"

class Stabilization: public Thread
{

public:
	Stabilization(ChordNode *n)
	: Thread(Debug::GetDebugger())
	, debug(Debug::GetDebugger())
	{ node = n; }

	void Run(void)
	{
		while (1)
		{
			std::cout << "Running the stabilization thread..." << std::endl;
			node->stabilize();
			node->fixFingersTable();
			node->checkPredecessor();
			this->SleepMs(node->getTimeToCheck());
		}
	}

private:
	ChordNode *node;
	std::shared_ptr<Debug> debug;
};

#endif
