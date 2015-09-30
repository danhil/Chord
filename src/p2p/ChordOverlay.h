#ifndef ABSTRACTCHORD_H
#define ABSTRACTCHORD_H

#include "Node.h"
#include "TransportCode.h"
#include "NetworkOverlay.h"
#include "Request.h"
#include <vector>
#include <map>

class Debug;

class ChordOverlay: public NetworkOverlay
{
public:

	ChordOverlay(int32_t addrSpace);

	void initialise(std::string ip, int32_t id, int32_t port);

public:

	int32_t spacesize;
	Node * findSuccessor(int32_t id);
	Node * closestPrecedingNode(int32_t id);
	void stabilize(void);
	void notify(Node* n);
	void fixFingersTable(void);
	void join(Node* chord);
	void checkPredecessor(void);

	virtual void checkStable(void) = 0;

	virtual std::string sendRequest(Request *request, Node* destination) = 0;

	std::string toString(void);

	Node * getThisNode(void) { return thisNode; }

	Node * getSuccessor(void){ return successor; }
	void setSuccessor(Node * n) { successor = n; }

	Node * getPredecessor(void) { return predecessor; }
	void	setPredecessor(Node * n) { predecessor = n; }

	int32_t		getTimeToCheck(void) { return timeToCheck; }
	void	setTimeToCheck(int32_t t) { timeToCheck = t; }

	/* Helper Methods */
	bool	insideRange(int id, int a, int b);
	std::string	printStatus();

protected:
	typedef std::map<int, std::string> stringMap;
	typedef std::map<int, std::string>::iterator stringMapIterator;
	typedef std::vector<Node *> nodesVector;

	Node * thisNode;
	Node * predecessor;
	Node * successor;
	nodesVector fingerTable;
	stringMap table;
	int32_t next;
	int32_t timeToCheck;

private:
	ChordOverlay(void){}
	std::shared_ptr<Debug> debug;
};

#endif
