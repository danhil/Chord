#include "Node.h"
#include "Request.h"
#include <stdio.h>
#include <stdlib.h>
#include "DebugUtil.h"


Node::Node(const std::string &nodeIp_i, uint32_t nodeId_i, uint32_t nodePort_i)
: nodeIP_m(nodeIp_i)
, nodeID_m(nodeId_i)
, nodePort_m(nodePort_i)
, debug(Debug::GetDebugger())
{
	char nidBuffer[255];
	sprintf(nidBuffer, "%d", nodeID_m);
	nodeIDString_m = std::string(nidBuffer);
}


Node::Node(const std::string &nodeIp_i, uint32_t nodePort_i)
: nodeIP_m(nodeIp_i)
, nodeID_m(0)
, nodePort_m(nodePort_i)
, debug(Debug::GetDebugger())
{
	//Transform the nodeID_m into a std::string for later usage.
	char nidBuffer[255];
	sprintf(nidBuffer, "%d", nodeID_m);
	nodeIDString_m = std::string(nidBuffer);
}


Node::Node(const std::string &nodeData_i)
: debug(Debug::GetDebugger())
{
	std::string inID, inIP, nodePort_i;
	int32_t at = nodeData_i.find("@");
	inID = nodeData_i.substr(0, at);
	int32_t ddot = nodeData_i.find(":");
	inIP = nodeData_i.substr(at+1, ddot-at-1);
	nodePort_i = nodeData_i.substr(ddot+1);

//	cout << "\n***inID=" << inID << endl;
//	cout << "***inIP=" << inIP << endl;
//	cout << "***nodePort_i=" << inPort << endl;

	this->nodeIDString_m = inID;
	this->nodeID_m = atoi(inID.c_str());
	this->nodeIP_m = inIP;
	this->nodePort_m = atoi(nodePort_i.c_str());
}


bool Node::equals(Node * node_i) {
		return node_i != nullptr
		  && ((node_i->getNodeId() == nodeID_m)
		  && (node_i->getNodePort() == nodePort_m)
			&& (node_i->getNodeIp() == nodeIP_m));

}


//@format  <ID>@<nodeIP_m>:<nodePort_m>
std::string Node::toString()
{
  std::ostringstream ss;
	ss << nodeIDString_m << "@" << nodeIP_m << ":" << nodePort_m;
	return (ss.str());
}
