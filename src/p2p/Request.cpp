#include "Request.h"
#include "TransportCode.h"
#include "DebugUtil.h"

Request::Request(const std::string &overlayID_i, const int32_t &code_i)
: debug(Debug::GetDebugger())
{
	this->overlayID_m = overlayID_i;
	this->code_m = code_i;
}

void Request::addArg(const std::string  &key_i, const std::string &value_i)
{
	arguments.insert(arg(key_i, value_i));
}

std::string Request::getArg(const std::string &key_i)
{
	argMap::iterator it = arguments.find(key_i);

	return it == arguments.end() ? "" : it->second;
}

uint32_t Request::getCheckSum(const std::string &str_i)
{

//	hash_state md;
//	unsigned char out[16];
//
//	//Initialize the MD5 hash function according to Tomcrypt library.
//	md5_init(&md);
//
//	//Apply SHA-1 to the input string.
//	md5_process(&md, (unsigned char *) str.c_str(), str.length());
//
//	//Get the hash output.
//	md5_done(&md, out);
//
//	// transform the md5 string to an integer
//	unsigned int md5toInt = 0;
//	for (int i = 0; i < strlen((const char *) out) + 1; i++)
//		md5toInt = md5toInt * 256 + (out[i] & 0xff);

	return 0;
}

/* Serialize the whole request into a callback string. "/callback?var1=a&var2=b....&var1000=bar" */
std::string Request::serialize()
{

  std::shared_ptr<std::string> callback = nullptr;

	switch (this->getCode())
	{
	  case GETPRED:
		  callback = std::make_shared<std::string>("/getpred");
		break;

	  case FINDSUCC:
		  callback = std::make_shared<std::string>("/findsucc");
		break;

	  case NOTIF:
		  callback = std::make_shared<std::string>("/notif");
		break;

	  case JOIN:
		  callback = std::make_shared<std::string>("/join");
		break;

	  case PUT:
		  callback = std::make_shared<std::string>("/put");
		break;

	  case GET:
		  callback = std::make_shared<std::string>("/get");
		break;

	  case REMOVEKEY:
		  callback = std::make_shared<std::string>("/removekey");
		break;

	  case SETSUCC:
		  callback = std::make_shared<std::string>("/setsucc");
		break;

	  case SETPRED:
		  callback = std::make_shared<std::string>("/setpred");
		break;

	  default:
      std::cerr << " UNHANDLED CHORD TRANSPORT CODE! ... ASSERTING" << std::endl;
		  assert(this->getCode());
		break;
	}

  // Create a stringstream and build the message
  std::ostringstream ss;
	ss << (*callback) << "?";

  // Append the argument list
	for (argMap::iterator it = arguments.begin(); it != arguments.end(); ++it)
  {
		ss << (*it).first << "=" << (*it).second << "&";
	}
  // Append the checksum
	ss << "checksum=" << getCheckSum(ss.str());

  std::cout << ss.str() << std::endl;

	return ss.str();
}
