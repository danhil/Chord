#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

#include "DebugUtil.h"
#include "ThreadUtil.h"

int main()
{
  std::cout << "Starting threadutil test. " << std::endl;
  // Create a thread
  Thread * t = new Thread(std::make_shared<Debug>("Thread", Debug::V, std::cout));
  // Get the thread ID
  t->GetThreadID();
}

