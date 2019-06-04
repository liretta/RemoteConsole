/*! 
 *united functionality server-side
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
	void run() override; //running data exchange between client and server while connection is present
    bool waitingForConnection(); //waiting in "accept" socket
	bool sendKey(); //generate and send public key to the client
	bool getKey(); //get synchronous key from client and initialize synchronous encryptor/decryptor
	bool reconnect(); //waiting for new client in "accept" socket
	void logIn(); //get log/pass, check it, return answer to client

private:
	bool client_log_in();
	bool data_exchange();

	ServerNetworker m_networker;
	ServerExecutor m_executor;
	ServerCryptor m_cryptor;
	ServerLogger m_logger;
	bool is_connection;
};

