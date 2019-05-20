#pragma once
#include "i_logger.h"
#include <fstream>
#include <unordered_map>
#include <string>
#include <functional>
#include <algorithm>

constexpr const char* FILE_USERS_NAME = "users.ini";
static size_t name_hash(const auth_data &name);

class ServerLogger:public iLogger
{
public:
	ServerLogger(ServerNetworker & networker) : m_networker(networker)
	{};

	bool check_password(auth_data const &log_pair, Access acs) override;

private:
	ServerNetworker &m_networker;
	bool load_auth_data_from_file(std::unordered_map<auth_data, int, decltype(&name_hash)>& auth_list);
};