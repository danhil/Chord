#ifndef CHECKPRED_H
#define CHECKPRED_H

#include "ThreadUtil.h"
#include "ChordNode.h"
#include "SingletonChord.h"

class CheckPredecessor: public Thread
{
public:
	CheckPredecessor(ChordNode * n)
	: Thread(Debug::GetDebugger())
	, node(n)
	{}

	void run()
	{
		while(true)
		{
			node->checkPredecessor();
			this->SleepMs(node->getTimeToCheck());
		}
	}

	ChordNode * node;
};

#endif
