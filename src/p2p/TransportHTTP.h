#ifndef TRANSPORT_HTTP_H
#define TRANSPORT_HTTP_H

#include "ITransport.h"
#include "mongoose.h"
#include <iostream>
#include <string>
#include <sstream>

class ChordNode;

class TransportHTTP: public ITransport
{
public:
	TransportHTTP(int32_t port, const std::string &root);

	~TransportHTTP();

	char* sendRequest(Request *request, Node *destination);

	std::string sendTrackerRequest(const std::string &host, int32_t port,
			const std::string &callback);

	std::string doStuff(const std::string &code);

	void test();

	void startHTTP();

	void stopHTTP();

	void initCallbacks();

	void addCallback(const char *name, void (*callback)(mg_connection*, const mg_request_info*, void*));

	std::string connectToTracker(std::string &ip, int32_t port, Node *n);

private:
	// The mongoose http context
	struct mg_context * ctx;

protected:
	std::string root;
};

#endif
