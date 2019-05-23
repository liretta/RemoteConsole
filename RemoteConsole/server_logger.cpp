#include "server_logger.h"

/*!
 * hash-function for pair login+password
 */
static size_t name_hash(const auth_data &name)
{
	return std::hash<std::string>() (name.first) ^ std::hash<std::string>()(name.second);
}

/*!
 * open file with authorization data and load it to unordered_map
 * file name is constexpr
 * file keep user's quantity as first record, than keep pair log/pass + access
 * @return true if reading and writing to container was successful
 * @return false if file doesn't exist or wasn't open
 */
bool ServerLogger::load_auth_data_from_file(std::unordered_map<auth_data, int, decltype(&name_hash)>& auth_list)
{
	std::ifstream in;
	in.open(FILE_USERS_NAME);
	if(!in.is_open())
	{
		return false;
	}

	int user_quantity = 0;
	in >> user_quantity;
	if (user_quantity == 0)
	{
		return false;
	}

	for (auto i = 0; i< user_quantity; ++i)
	{
		std::string tmp_login, tmp_pass;
		int tmp_access;
		in >> tmp_login >> tmp_pass >> tmp_access;
		auth_list[auth_data(tmp_login, tmp_pass)] = tmp_access;
	}
	return true;
}

/*!
 * check sending data with data from file
 * get pair log+pass and kind of access
 * @return true if there are such login, password and access in file 
 * @return false if there isn't such login, or password/access for this login isn't appropriate
 */
bool ServerLogger::check_password(std::pair<std::string, std::string> const &log_pair, Access acs)
{
	
	std::unordered_map<auth_data, int, decltype(&name_hash)> user_list(0, name_hash);
	if (!load_auth_data_from_file(user_list))
	{
		return false;
	}

	const auto it = std::find_if(user_list.begin(), user_list.end(), [&log_pair, &acs] (std::pair<const auth_data, int>& elem)
	{
		return (elem.first.first == log_pair.first
			&& elem.first.second == log_pair.second
			&& elem.second == acs);
	});

	return (!(it == user_list.end()));
}

/*!
 * check sending data with data from file
 * get pair string log+pass
 * @return true if there are such login, password and access in file
 * @return false if there isn't such login, or password/access for this login isn't appropriate
 */
bool ServerLogger::check_password(const std::string &log, const std::string &pass)
{
	auth_data log_pair = std::make_pair(log, pass);
	Access acs = USER;
	std::unordered_map<auth_data, int, decltype(&name_hash)> user_list(0, name_hash);
	if (!load_auth_data_from_file(user_list))
	{
		return false;
	}

	const auto it = std::find_if(user_list.begin(), user_list.end(), [&log_pair, &acs](std::pair<const auth_data, int>& elem)
	{
		return (elem.first.first == log_pair.first
			&& elem.first.second == log_pair.second
			&& elem.second == acs);
	});

	return (!(it == user_list.end()));

}