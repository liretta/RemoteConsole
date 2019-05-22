#pragma once
#include "i_server.h"

class Server: public IServer
{
public:
	ServerExecutor& getExecutor() override;
	ServerLogger& getLogger() override;
	ServerNetworker& getNetworker() override;
	void Run() override;

private:
	bool client_log_in();

	ServerNetworker m_networker;
	ServerExecutor m_executor;
	ServerCryptor m_cryptor;
	ServerLogger m_logger;

};

