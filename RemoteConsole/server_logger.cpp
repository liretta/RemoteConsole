#include "server_logger.h"

///*!
// * hash-function for pair login+password
// */
//static size_t name_hash(const auth_data &name)
//{
//	return std::hash<std::string>() (name.first) ^ std::hash<std::string>()(name.second);
//}
static unsigned long hash(unsigned char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

/*!
 * open file with authorization data and load it to unordered_map
 * file name is constexpr
 * file keep user's quantity as first record, than keep pair log/pass + access
 * @return true if reading and writing to container was successful
 * @return false if file doesn't exist or wasn't open
 */
bool ServerLogger::load_auth_data_from_file(std::unordered_map < std::string, unsigned long>& auth_list)
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

	for (auto i = 0; i < user_quantity; ++i)
	{
		std::string tmp_login;
		unsigned long tmp_pass;
		in >> tmp_login >> tmp_pass;
		auth_list[tmp_login] = tmp_pass;
	}
	return true;
}

/*!
 * check sending data with data from file
 * get pair log+pass and kind of access
 * @return true if there are such login, password and access in file 
 * @return false if there isn't such login, or password/access for this login isn't appropriate
 */
bool ServerLogger::check_password(std::pair<std::string, std::string> const &log_pair)
{
	
	std::unordered_map<std::string, unsigned long> user_list;
	if (!load_auth_data_from_file(user_list))
	{
		std::cerr << "Cannot open file with authorization data\n";
		return false;
	}

	const auto it = user_list.find(log_pair.first);
	if (it != user_list.end())
	{
		return (hash((unsigned char*)log_pair.second.c_str()) == user_list[log_pair.first]);
	}
	return false;
}

/*!
 * check sending data with data from file
 * get pair string log+pass
 * @return true if there are such login, password and access in file
 * @return false if there isn't such login, or password/access for this login isn't appropriate
 */
bool ServerLogger::check_password(const std::string &log, const std::string &pass)
{
	return(check_password(std::make_pair(log, pass)));
}