#include "client_logger.h"

bool ClientLogger::check_password(auth_data const &log_pair, Access acs)
{
	std::string tmp_str = log_pair.first + " " + log_pair.second;
	m_networker.send(tmp_str);
	if (m_networker.receive() != "#Error")
	{
		return true;
	}
	return false;
}