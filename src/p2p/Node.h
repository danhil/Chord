#ifndef _node_h
#define _node_h

#include <string>
#include <iostream>
#include <vector>

class Debug;

class Node
{

public:
	Node(const std::string &nodeIp_i, uint32_t nodeId_i, uint32_t nodePort_i);
	Node(const std::string &nodeIP_m, uint32_t nodePort_m);
	Node(const std::string &data);

	bool equals(Node * node);

  std::string	toString(void);

  std::vector<std::string> tokenize(const std::string & str, const std::string & delims);

	uint32_t getNodeId(void)
	{
	  return nodeID_m;
	}

	void setNodeId(uint32_t nodeId_i)
	{
	  nodeID_m = nodeId_i;
	}

  std::string getNodeIdString(void)
  {
    return nodeIDString_m;
  }

	void setNodeIdString(const std::string & idString_i)
	{
	  nodeIDString_m = std::string(idString_i);
	}

  std::string getNodeIp(void)
  {
    return nodeIP_m;
  }

	void setNodeIp(const std::string nodeIp_i)
	{
	  nodeIP_m = nodeIp_i;
	}

	int32_t getNodePort(void)
	{
	  return nodePort_m;
	}

	void setNodePort(int32_t nodePort_i)
	{
	  nodePort_m = nodePort_i;
	}

private:
  std::string	nodeIP_m;
  std::string nodeIDString_m;
	uint32_t nodeID_m;
  int32_t	nodePort_m;
	std::shared_ptr<Debug> debug;

};

#endif
