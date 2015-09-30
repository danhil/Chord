#define P_SINGLETON SingletonChord::getInstance()

#ifndef PROTOCOL_SINGLETON_H
#define PROTOCOL_SINGLETON_H

#include "ChordNode.h"
#include <string>

class Debug;

class SingletonChord
{
public:

	static SingletonChord* getInstance(void);

	~SingletonChord();

	ChordNode *	initChordNode(const std::string &ip, int32_t port, const std::string &overlayIntifier, const std::string &rootDirectory);
	void stopChord(void);

	void		setProtocolNode(ChordNode * n) { myNode = n;}
	ChordNode*	getChordNode(void) { return myNode; }

protected:
	SingletonChord(void);

private:
	ChordNode * myNode;
	static SingletonChord * instance;
	std::shared_ptr<Debug> debug;
};

#endif
