#include "SingletonChord.h"
#include "DebugUtil.h"

SingletonChord* SingletonChord::instance = NULL;

SingletonChord::SingletonChord()
: debug(Debug::GetDebugger())
{
	myNode = NULL;
}

SingletonChord::~SingletonChord()
{
	delete myNode;
	delete instance;
}

// Not yet threadsafe
SingletonChord* SingletonChord::getInstance()
{
	if (instance == NULL)
	{
		instance = new SingletonChord();
	}
	return instance;
}

ChordNode* SingletonChord::initChordNode(const std::string &ip
	, int port
	, const std::string &overlayIntifier
	, const std::string &rootDirectory)
{
	myNode = new ChordNode(ip, port, overlayIntifier, rootDirectory);
	return myNode;
}

void SingletonChord::stopChord()
{
	delete myNode;
	myNode = NULL;
}
