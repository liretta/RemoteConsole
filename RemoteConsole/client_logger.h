/*!
 * get log+pass, send it to the server, and return result
 */
#pragma once
#include "i_logger.h"
#include "marshaller.h"
#include "client_cryptor.h"
#include "converting_functions.hpp"

class ClientLogger : public iLogger
{
public:
	ClientLogger(ClientNetworker &networker, ClientCryptor &cryptor) : m_networker(networker), m_cryptor(cryptor)
	{};

	bool check_password(auth_data const &log_pair, Access acs) override;
	bool check_password(const std::string &log, const std::string &pass) override;

private:
	ClientNetworker &m_networker;
	ClientCryptor &m_cryptor;
};