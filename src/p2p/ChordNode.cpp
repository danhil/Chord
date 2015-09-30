#include "TransportHTTP.h"
#include "ChordNode.h"
#include "NetworkStabilization.h"
#include "CheckPredecessor.h"
#include "FixFinger.h"
#include "callbacks.h"
#include "sha1.h"
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <dirent.h>
#include <assert.h>
#include <math.h>
#include <DebugUtil.h>

static const std::string PERSISTENCE_DIR(".chord/data");
static const std::string PERSISTENCE_DIR_STRING(".chord/data/%s");

ChordNode::ChordNode(const std::string &ip, uint32_t port, const std::string &overlayIdentifier, const std::string &rootDirectory)
: ChordOverlay(9)
, notified(false)
, debug(Debug::GetDebugger())
{
	// Create the id
	std::ostringstream oss;
	oss << ip << port;
	// Get the sha i of the ip + port
	uint32_t id = getIntSHA1(oss.str());
	// check if the idE[0, 2^(spacesize - 1)]
	assert(!(id > pow(2, spacesize)) && !(id < 0));
	// Create the stabilizer
	stableThread = new Stabilization(this);
	//Initialize the transport layer.
	transport = new TransportHTTP(port, rootDirectory);
	//set the overlay identifier.
	this->overlayIdentifier = overlayIdentifier;
	//Call our parent's initializer
	initialise(ip, id, port);
	//We start-up our stabilizer thread.
	checkStable();
}

ChordNode::~ChordNode()
{
	stableThread->Kill();
	delete stableThread;
	delete transport;
}

void ChordNode::notify(Node * n)
{
	Node *pred = predecessor;
	ChordOverlay::notify(n);
	// If the predecessor as changed, update the DHT table
	if (pred != predecessor)
	{
		notified = true;
	}
}

/* Custom chord stabilize */
void ChordNode::stabilize()
{
	((ChordOverlay *) this)->stabilize();
	// If the predecessor as changed, update the DHT table
	if (notified && predecessor->getNodeId() != thisNode->getNodeId())
	{
		struct dirent *dirEntry;
		char path[256];
		DIR *dir = opendir(PERSISTENCE_DIR.c_str());
		if(dir != NULL)
		{
			while(dirEntry=readdir(dir))
			{
				if(dirEntry->d_type != DT_DIR)
				{
					uint32_t hFilename = getIntSHA1(dirEntry->d_name);
					if (!insideRange(hFilename, predecessor->getNodeId(), thisNode->getNodeId()))
					{
						Request *request = new Request(this->getIdentifier(), PUT);
						request->addArg("key", dirEntry->d_name);
						request->addArg("value", openData(dirEntry->d_name));
						sendRequest(request, predecessor);
						delete request;
						sprintf(path, PERSISTENCE_DIR_STRING.c_str(), dirEntry->d_name);
						if( remove( path ) != 0 )
						{
							std::cerr << "Error removing path." << std::endl;
						}
					}
				}
			}
		}
		notified = false;
	}
}

/* find and replace std::string tools */
std::string find_and_replace(std::string str, const std::string find, std::string replace)
{
	size_t j;
	std::string source = str;
	for (; (j = source.find(find)) != std::string::npos;)
	{
		source.replace(j, find.length(), replace);
	}
	return source;
}

/* serialize a postit before to store it into the DHT */
std::string ChordNode::serialize(std::string data)
{
	data = find_and_replace(data, " ", "\\_");
	data = find_and_replace(data, "\t", "\\t");
	data = find_and_replace(data, "\n", "\\n");
	return data;
}

/* reconstruct a serialized postit */
std::string ChordNode::unserialize(std::string data)
{
	data = find_and_replace(data, "\\_", " ");
	data = find_and_replace(data, "\\t", "\t");
	data = find_and_replace(data, "\\n", "\n");
	return data;
}

/* save the data value in a text file */
void ChordNode::saveData(std::string filename, std::string value)
{
	char path[256];
	sprintf(path, PERSISTENCE_DIR_STRING.c_str(), filename.c_str());
	mkdir(".chord/", 0777);
	mkdir(".chord/data", 0777);
	std::ofstream data(path, std::ios::out);
	data << unserialize(value);
}

/* return the content of the file */
std::string ChordNode::openData(std::string filename)
{
	std::string line, data="";
	char path[256];
	sprintf(path, PERSISTENCE_DIR_STRING.c_str(), filename.c_str());
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		while (! myfile.eof())
		{
			getline(myfile,line);
			data.append(line.c_str());
			if(! myfile.eof())
			{
				data.append("\n");
			}
		}
		myfile.close();
	}
	else return "null";
	return data;
}

/* DHT Put */
void ChordNode::put(std::string key, std::string value)
{
	// Convert the key in a hash integer
	uint32_t hKey = getIntSHA1(key);
	if (insideRange(hKey, predecessor->getNodeId() + 1, thisNode->getNodeId()))
	{
		saveData(key, value);
	}
	else
	{
		// Find the node responsible for this key
		Node *responsible = findSuccessor(hKey);
		// Create a Put request.
		Request *request = new Request(this->getIdentifier(), PUT);
		request->addArg("key", key);
		request->addArg("value", value);
		// Send the Put request
		sendRequest(request, responsible);
		delete request;
	}
}

/* DHT Get */
std::string ChordNode::get(std::string key)
{
	// Convert the key in a hash integer
	uint32_t hKey = getIntSHA1(key);
	if (insideRange(hKey, predecessor->getNodeId() + 1, thisNode->getNodeId()))
	{
		return openData(key);
	}
	else
	{
		// Find the node responsible for this key
		Node *responsible = findSuccessor(hKey);
		// Create a Put request.
		Request *request = new Request(this->getIdentifier(), GET);
		request->addArg("key", key);
		// Send the Put request
		sendRequest(request, responsible);
		delete request;
	}
}

/* DHT Remove */
void ChordNode::removekey(std::string key)
{
	// Convert the key in a hash integer
	uint32_t hKey = getIntSHA1(key);
	if (insideRange(hKey, predecessor->getNodeId() + 1, thisNode->getNodeId()))
	{
		char path[256];
		sprintf(path, PERSISTENCE_DIR_STRING.c_str(), key.c_str());
		if( remove( path ) != 0 )
		{
			std::cerr << "Error deleting file." << std::endl;
		}
	}
	else // Not resposible for the key, find the correct node
 	{
		Node *responsible = findSuccessor(hKey);
		Request *request = new Request(this->getIdentifier(), REMOVEKEY);
		request->addArg("key", key);
		sendRequest(request, responsible);
		delete request;
	}
}

/* Convert a std::string into an integer in [0, spacesize] */
uint32_t ChordNode::getIntSHA1(std::string str)
{
	SHA1 *sha1 = new SHA1();
	sha1->addBytes( str.c_str(), strlen( str.c_str() ));
	unsigned char* digest = sha1->getDigest();
	uint32_t res = sha1->shaToInteger(digest, 20, pow(2, spacesize));
	delete sha1;
	free( digest );
	return res;
}

/* Convert a std::string into a std::string (using SHA1) */
char *ChordNode::getHexSHA1(std::string str)
{
	SHA1 *sha1 = new SHA1();
	sha1->addBytes( str.c_str(), strlen( str.c_str() ));
	unsigned char *digest = sha1->getDigest();
	char *h = sha1->hexPrinter(digest, 20);
	delete sha1;
	free( digest );
	return h;
}

/* Forward a message to a peer, the message is in the format: "<IP+PORT>,TRANSPORT_CODE" */
std::string ChordNode::sendRequest(Request * request, Node * destination)
{
	std::cout << "Sending request." << std::endl;
	// Send the request to the peer
	char * response = transport->sendRequest(request, destination);
	// response received
	if (response)
	{
		std::cout << "Got response for the request." << std::endl;
		std::stringstream ss;
		ss << response;
		free(response); // we must free the initial char* response, to avoid leaks.
		return ss.str();
	}
	else
	{
		std::cout << "Did not get respnse from the peer." << std::endl;
		// No peer - update the pointers
		fixBrokenPointers(destination);
		// time to fix the chord
		sleep(1);
		// The node is completely disconnected of the backbone
		if (isAlone())
		{
			return getThisNode()->toString();
		}
		// Try the request with a new destination
		return sendRequest(request, findSuccessor(destination->getNodeId()));
	}
}

/* Fix broken pointers algorithm */
void ChordNode::fixBrokenPointers(Node *node)
{
	for (uint32_t i = 0; i < fingerTable.size() - 1; i++)
	{
		if (fingerTable[i]->getNodeId() == node->getNodeId())
		{
			fingerTable[i] = new Node(thisNode->toString());
		}
	}
	if (predecessor->getNodeId() == node->getNodeId())
	{
		predecessor = new Node(thisNode->toString());
	}
	if (successor->getNodeId() == node->getNodeId())
	{
		successor = new Node(thisNode->toString());
	}
}

/* return true if the node is completely disconnected of the chord */
bool ChordNode::isAlone()
{
	for (uint32_t i = 0; i < fingerTable.size() - 1; i++)
	{
		if (fingerTable[i]->getNodeId() != thisNode->getNodeId())
		{
			return false;
		}
	}
	return predecessor->getNodeId() == thisNode->getNodeId() && successor->getNodeId() == thisNode->getNodeId();
}

/* Starts up the "stabilizer thread" for this peer. */
void ChordNode::checkStable()
{
	stableThread->Start();
}

/* Stop the stabilization, distribute the key and shutDown the peer */
void ChordNode::shutDown()
{
	// Kill stabilization
	stableThread->Kill();
	// notify predecessor
	Request *request = new Request(this->getIdentifier(), SETSUCC);
	request->addArg("successor", successor->toString());
	sendRequest(request, predecessor);
	delete request;
	// notify successor
	request = new Request(this->getIdentifier(), SETPRED);
	request->addArg("predecessor", predecessor->toString());
	sendRequest(request, successor);
	delete request;
	// if the node is not alone
	if(successor->getNodeId() != getThisNode()->getNodeId())
	{
		// give the part of the DHT to the successor
		struct dirent *dirEntry;
		DIR *dir = opendir(PERSISTENCE_DIR.c_str());
		char path[256];
		while(dirEntry=readdir(dir))
		{
			if(dirEntry->d_type != DT_DIR)
			{
				request = new Request(this->getIdentifier(), PUT);
				request->addArg("key", dirEntry->d_name);
				request->addArg("value", openData(dirEntry->d_name));
				sendRequest(request, successor);
				delete request;
				sprintf(path, PERSISTENCE_DIR_STRING.c_str(), dirEntry->d_name);
				if( remove( path ) != 0 )
				{
					perror( "Error deleting file" );
				}
			}
		}
	}
	// leave
	std::cout << "Shutting down the chord node...\n";
	sleep(1);
	exit(0);
}
