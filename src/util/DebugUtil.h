#ifndef _debug_util_h
#define _debug_util_h

#include <iostream>
#include <sstream>
#include <functional>
#include <memory>

class Debug : public std::ostream, public std::stringbuf {

public:

  typedef enum {Q, E, W, I, V } DebugLevel;

private:

  typedef std::function <std::string (std::string debugName, std::string level, std::string data)> DebugFormatter;
  // Debug debugLabels
  const char * debugLabels[4] = { "Error", "Warning", "Information", "Verbose" };
  // The output stream
  std::ostream &output;
  // The <optional> debugFormatter
  DebugFormatter debugFormatter;
  // The debugName of this logger
  std::string debugName;
  // The maximum level that is printed
  DebugLevel maxLevel;
  // The level of the current debug output
  DebugLevel currentLevel;

public:

  Debug(std::string debugName, DebugLevel maxLevel, std::ostream& stream) :
    std::ostream(this)
    , maxLevel(maxLevel)
    , debugName(debugName)
    , output(stream)
    , debugFormatter(nullptr)
    , currentLevel(V){}

  // Override the ostream sync method to append the debug debugName and level.
  int32_t sync (void)
  {
    if (currentLevel <= maxLevel)
    {
      if (debugFormatter)
      {
        std::string level = debugLabels[currentLevel - 1];
        output << debugFormatter(debugName, level, str());
      }
      else
      {
        output << debugName
          << " "
          << debugLabels[currentLevel - 1]
          << " "
          << str();
      }
    }

    str("");
    // Flush the stream
    output.flush();
    return 0;
  }

  // Set the format lambda to be called on debugging
  void SetFormatter(Debug::DebugFormatter f)
  {
    debugFormatter = f;
  }

  static std::shared_ptr<Debug> GetDebugger(void)
  {
    static Debug * debugger = new Debug("Global debugger", Debug::W, std::cout);
    return std::shared_ptr<Debug>(debugger);
  }

  // Overide the << operator to get the level of the debug
  friend Debug & operator << (Debug & d, const Debug::DebugLevel & level)
  {
    d.currentLevel = level;
    return d;
  }

};

#endif
