/* !
 * united functionality client-side
 * get opportunity to logIn, execute command and exchange keys
 */
#pragma once
#include "i_client.h"

class Client : public IClient
{
public:
	ClientExecutor& getExecutor()  override;
	ClientLogger& getLogger() override;
	ClientNetworker& getNetworker()  override;
	bool setCryptoKey() override; //receive key from server, validate and save it 
	bool sendCryptoKey() override; //generate synchronous key and send it to the server
	Client();
	~Client() = default;

private:
	ClientNetworker m_networker;
	ClientExecutor m_executor;
	ClientCryptor m_cryptor;
	ClientLogger m_logger;
};

