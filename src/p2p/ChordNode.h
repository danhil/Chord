#ifndef _ChordNode_H
#define _ChordNode_H

#include <stdint.h>
#include "NetworkOverlay.h"
#include "ChordOverlay.h"
#include "TransportHTTP.h"
#include <string>
#include <map>
#include <memory>

// Class declarations
class Stabilization;
class Debug;
// Concrete implementation of a chord protocol node
class ChordNode: public ChordOverlay
{

public:
	ChordNode(const std::string &ip
		, uint32_t port
		, const std::string &overlayIdentifier
		, const std::string &rootDirectory);

	~ChordNode();

	virtual std::string sendRequest(Request * request, Node * destination);

	virtual void fixBrokenPointers(Node * node);

	virtual bool isAlone(void);

	virtual void checkStable(void);

	virtual void shutDown(void);
	// @Override
	void notify(Node *n);
	// @Override
	void stabilize(void);

	void put(std::string key, std::string value);

	std::string get(std::string key);

	void removekey(std::string key);

	void saveData(std::string filename, std::string value);

	std::string openData(std::string filename);

	std::string serialize(std::string data);

	std::string unserialize(std::string data);

	unsigned int   getIntSHA1(std::string key);

	char *getHexSHA1(std::string str);

	std::string getIdentifier(void)
	{
		return overlayIdentifier;
	}

	TransportHTTP* getTransport(void)
	{
		return transport;
	}

	Node* getThisNode(void)
	{
		return thisNode;
	}

	void setIdentifier(const std::string &iD)
	{
		overlayIdentifier = iD;
	}

	void setTransport(TransportHTTP * t)
	{
		 transport = t;
	}

private:
	/* the transport layer */
	TransportHTTP* transport;
	/* One thread for the stabilization of each node */
	Stabilization* stableThread;
	/* The id of the overlay */
	std::string overlayIdentifier;
	/* node notification */
	bool notified;
	/* Part of the DHT */
	typedef std::pair<std::string, std::string> data;
	typedef std::map<std::string, std::string> dataMap;
	dataMap table;
	// Debugger
	std::shared_ptr<Debug> debug;
};

#endif
