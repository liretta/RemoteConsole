/*! 
 *united functionality server-side
 * all work are doing by method run() in endless cycle
 */
#pragma once
#include "i_server.h"



class Server: public IServer
{
public:
	Server();
	~Server() = default;
	ServerExecutor& getExecutor() override;
	ServerLogger& getLogger() override;
	ServerNetworker& getNetworker() override;
	void run() override;
    bool waitingForConnection();

private:
	bool client_log_in(bool &is_connection);
	bool data_exchange();

	ServerNetworker m_networker;
	ServerExecutor m_executor;
	ServerCryptor m_cryptor;
	ServerLogger m_logger;
};

