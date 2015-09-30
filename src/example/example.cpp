#include <iostream>
#include <string>
#include <stdlib.h>
#include <ChordNode.h>
#include <SingletonChord.h>
#include <pthread.h>


// @argv : ip port overlay identifier (unique std::string), "root directory)"
int main(int argc, char * const argv[])
{
	std::string backBone[] = { "127.0.0.1",};

  Node * chord = NULL;
  ChordNode *node = NULL;

	if (argc >= 4)
	{
		// Create a test node
			node = SingletonChord::getInstance()->initChordNode(std::string(argv[1]), atoi(argv[2]), std::string("ChordTestBed"), std::string(argv[3]));

		// join to an existing chord
		if (argc == 5)
		{
			std::cout << "joining...\n";
			int i = 0;
		  chord = new Node(backBone[0], 8000);
			node->join(chord);
		}

		char entry[256];
		std::string key;
		std::string value;

		while (1)
		{ // sleep...
			std::cout << "\n0) Print status\n" <<
					"1) Put\n" <<
					"2) Get\n" <<
					"3) Remove\n" <<
					"4) Exit\n\n";
			std::cout << "---> ";
			std::cin >> entry;
			int chx = atoi(entry);

			switch (chx)
			{
    			case 0:
    				std::cout << "\n" << node->printStatus();
    				break;
    			case 1:
    				std::cout << "Key = ";
    				std::cin >> key;
    				std::cout << "Value = ";
    				std::cin >> value;
    				node->put(key, value);
    				break;
    			case 2:
    				std::cout << "Key = ";
    				std::cin >> key;
    				std::cout << "\n" << node->get(key) << "------> found!" << std::cout;
    				break;
    			case 3:
    				std::cout << "Key = ";
    				std::cin >> key;
    				node->removekey(key);
    				break;
    			case 4:
    				node->shutDown();
    			default:
				break;
			}
	  } /* WHILE */
  }
	else
	{
		std::cout << "wrong parameters: " << argv[0] << " <hostname> <portNumber> <webContentDirectory> [--join]" << std::endl;
	}

  delete node;
  delete chord;
	return 0;
}
