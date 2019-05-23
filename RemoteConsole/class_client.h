/* !
 * united functionality client-side
 * get opportunity to logIn, Excecut command and encryption message
 */
#pragma once
#include "i_client.h"

class Client : public IClient
{
public:
	ClientExecutor& getExecutor()  override;
	ClientLogger& getLogger()  override;
	ClientNetworker& getNetworker()  override;
	Client();
	~Client() = default;

private:
	ClientNetworker m_networker;
	ClientExecutor m_executor;
	ClientCryptor m_cryptor;
	ClientLogger m_logger;
};

