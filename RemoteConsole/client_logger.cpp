#include "client_logger.h"

bool ClientLogger::check_password(auth_data const &log_pair, Access acs)
{
	std::wstring pack_message = Marshaller::packAuthorizationData(STRINGtoWSTRING(log_pair.first), STRINGtoWSTRING(log_pair.second));
	std::string str = WSTRINGtoSTRING(pack_message);
	m_networker.send(str);
	std::string str_res = m_networker.receive();

	if (str_res == "#Error")
	{
		return false;
	}
	
	return Marshaller::unpackResult(STRINGtoWSTRING(str_res));
}