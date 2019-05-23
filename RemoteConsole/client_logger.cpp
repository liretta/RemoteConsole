#include "client_logger.h"

/*!
 * get pair log+pass, pack it, encrypt and send to the server
 * @return false if connection have been lost 
 * @return result of checking log/pass
 */
bool ClientLogger::check_password(auth_data const &log_pair, Access acs)
{
	std::wstring pack_message = Marshaller::packAuthorizationData(STRINGtoWSTRING(log_pair.first), STRINGtoWSTRING(log_pair.second));
	m_networker.send(m_cryptor.encrypt(pack_message));
	std::vector<char> vc;
	bool result = m_networker.receive(vc);
	if (result == false)
	{
		PrintError(ERR_CONNECTION_LOST);
		return result;
	}
	
	return Marshaller::unpackResult(m_cryptor.decrypt(vc));
}

/*!
 * get separate string log+pass, pack it, encrypt and send to the server
 * @return false if connection have been lost
 * @return result of checking log/pass
 */
bool ClientLogger::check_password(const std::string &log, const std::string &pass)
{
	std::wstring pack_message = Marshaller::packAuthorizationData(STRINGtoWSTRING(log), STRINGtoWSTRING(pass));
	m_networker.send(m_cryptor.encrypt(pack_message));
	std::vector<char> vc;
	bool result = m_networker.receive(vc);
	if (result == false)
	{
		return result;
	}

	return Marshaller::unpackResult(m_cryptor.decrypt(vc));
}