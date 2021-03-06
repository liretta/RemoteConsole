/*! 
 * interface class for logger 
 */
#pragma once
#include <fstream>
#include <unordered_map>
#include "client_networker.h"
#include "server_networker.h"

typedef std::pair<std::string, std::string> auth_data; //login+password

class iLogger
{
public:
	virtual bool check_password(auth_data const &log_pair) = 0;
	virtual bool check_password(const std::string &log, const std::string &pass) = 0;
};
