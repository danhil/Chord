#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

#include "DebugUtil.h"

int main() {


  // Logging to a stream
  std::stringstream ss;
  Debug a("Stream test ", Debug::V, std::cout);
  // Out to the stream
  a << a.I << "Testing the debuglogging to std::cout." << std::endl;

  // Logging to a file...
  std::ofstream logfile;
  logfile.open("debugtest.log", std::ios::app);
  // Set up the debug stream
  Debug b("LogFile test ", Debug::V, logfile);
  //Log
  b << b.W << "Testing logging to an output file." << std::endl;
  // Close the logfile
  logfile.close();


  Debug c("Formatted test", Debug::V, std::cout);

  c.SetFormatter([=](std::string name,std::string level, std::string data)->std::string
                 { return name + " - " + level + " - " + data;});

  c << c.W << "Logging to a formatted output stream." << std::endl;
}

