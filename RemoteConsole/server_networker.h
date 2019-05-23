/*! 
 * create connecting with clients
 */
#pragma once
#include "base_networker.h"
//TODO: do it multi-thread - create vector with sockets and create new thread when new client is connecting

class ServerNetworker : public BaseNetworker
{
public:
	ServerNetworker(): m_listen_socket(INVALID_SOCKET)
	{};

	~ServerNetworker();
	Error init(const std::string &def_adr = "127.0.0.1") override;
	

private:
	SOCKET m_listen_socket;
	bool create_socket(const std::string &def_adr) override;//create listenSocket, connectSocket(for client), bind and start to listen
	bool create_connection() override; //accept client socket
};
