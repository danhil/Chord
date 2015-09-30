#include "ChordOverlay.h"
#include "SingletonChord.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <DebugUtil.h>

ChordOverlay::ChordOverlay(int32_t addrSpace)
: debug(Debug::GetDebugger())
{
	spacesize = addrSpace;
}

void ChordOverlay::initialise(std::string ip, int32_t id, int32_t port)
{
	std::cout << "Initializeing the chord node..." << std::endl;
	thisNode = new Node(ip, id, port);
	successor = thisNode;
	predecessor = thisNode;
	next = 0;
	timeToCheck = 250;

	for (int32_t i = 0; i < spacesize; i++)
	{
		fingerTable.push_back(thisNode);
	}
}


Node* ChordOverlay::findSuccessor(int32_t id)
{
	Node * successor_node = nullptr;

	if (insideRange(id, thisNode->getNodeId() + 1, successor->getNodeId()))
	{
		successor_node = successor;
	}
	else
	{
		Node *pred = closestPrecedingNode(id);

		//Forge the message that we will sendRequest (FINDSUCC)
		std::ostringstream oss;
		oss << id;

		//Create a request.
		Request * request = new Request(this->getIdentifier(), FINDSUCC);
		request->addArg("id", oss.str());

		std::string succ = sendRequest(request, pred);

		successor_node = new Node(succ);

		delete request;
	}

	return successor_node;
}


Node* ChordOverlay::closestPrecedingNode(int32_t nid)
{
	Node * prec_node = nullptr;
	if (thisNode == successor)
	{
		prec_node = thisNode;
	}
	else
	{
		for (int32_t i = fingerTable.size() - 1; i > 0; i--)
		{
			if (insideRange(fingerTable[i]->getNodeId(), thisNode->getNodeId() + 1, nid - 1))
			{
				prec_node = fingerTable[i];
				break;
			}
		}
	}

	return prec_node == nullptr ? successor : prec_node;
}


void ChordOverlay::join(Node * chord)
{
	Request * request = new Request(this->getIdentifier(), FINDSUCC);
	request->addArg("overlay_id", this->getIdentifier());
	request->addArg("id", thisNode->getNodeIdString());

	std::cout << "Sending a join request..." << std::endl;
	// Send the request
	std::string succ = this->sendRequest(request, chord);
	// Update the successor
	successor = new Node(succ);
	// Delete the request
	delete request;
}


void ChordOverlay::stabilize()
{
	std::cout << "Sending a stabilzation request ..." << std::endl;
	// Create the reqeust
	Request * pred_request = new Request(this->getIdentifier(), GETPRED);
	// Send the request
	std::string pred = sendRequest(pred_request, successor);
	// Create the new node
	Node * x = new Node(pred);

	bool inside = insideRange(x->getNodeId()
		, thisNode->getNodeId() + 1
		, successor->getNodeId() - 1);

	if (x->getNodeId() != thisNode->getNodeId() && inside )
	{
		successor = x;
	}

	// Notify
	Request * notif_request = new Request(this->getIdentifier(), NOTIF);
	notif_request->addArg("node", thisNode->toString());
	sendRequest(notif_request, successor);

	delete pred_request;
	delete notif_request;
}


void ChordOverlay::notify(Node * node)
{
	bool inside = insideRange(node->getNodeId()
		, predecessor->getNodeId() + 1
		, thisNode->getNodeId() - 1);

	if ((predecessor->getNodeId() == thisNode->getNodeId()) || inside)
	{
		predecessor = node;
	}
}


void ChordOverlay::fixFingersTable()
{
	std::cout << "Fixing the fingerstable." << std::endl;
	if (++next > spacesize)
	{
		next = 1;
	}
		// Set the fingerstable
	fingerTable[next - 1] = findSuccessor((thisNode->getNodeId() + (int32_t) pow(2,
			next - 1)) % (int32_t) pow(2, spacesize));
}

void ChordOverlay::checkPredecessor()
{
	Request *request = new Request(this->getIdentifier(), GETPRED);
	sendRequest(request, predecessor);
	delete request;
}

bool ChordOverlay::insideRange(int32_t id, int32_t begin, int32_t end)
{
	int32_t MAXid = pow(2, spacesize);
	int32_t MINid = 0;

return (begin < end && begin <= id && id <= end)
		|| (begin > end && ((begin <= id && id <= MAXid)
		|| (MINid <= id && id <= end)))
		|| ((begin == end) && (id == begin));
}

std::string ChordOverlay::printStatus()
{
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	ss << getIdentifier() << " on " << thisNode->getNodeIp() << ":"
			<< thisNode->getNodePort() << "\n" << "<NODE: " << thisNode->getNodeId()
			<< ", PRED: " << predecessor->getNodeId() << ", SUCC: "
			<< successor->getNodeId() << ">\n" << "\tFingers Table: [";

	for (int32_t i = 0; i < fingerTable.size() - 1; i++)
	{
		ss << fingerTable[i]->getNodeId() << ", ";
	}

	ss << fingerTable[fingerTable.size() - 1]->getNodeId() << "]\n\n";

	return ss.str();
}
