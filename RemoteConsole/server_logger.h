/*!
 * class for checking logIn data from external file
 */
#pragma once
#include "i_logger.h"
#include <fstream>
#include <unordered_map>
#include <string>
//#include <functional>
//#include <algorithm>

constexpr const char* FILE_USERS_NAME = "user.ini";
//static size_t name_hash(const auth_data &name);
static unsigned long hash(unsigned char *str);

class ServerLogger:public iLogger
{
public:
	ServerLogger(ServerNetworker & networker) : m_networker(networker)
	{};

	bool check_password(auth_data const &log_pair) override;
	bool check_password(const std::string &log, const std::string &pass) override;


private:
	ServerNetworker &m_networker;
	bool load_auth_data_from_file(std::unordered_map<std::string, unsigned long>& auth_list);
};