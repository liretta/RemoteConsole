#pragma once
#include "base_networker.h"
//TODO: do it multi-thread - create vector with sockets and create new thread when new client is connecting

class ServerNetworker : public BaseNetworker
{
public:
	ServerNetworker();
	~ServerNetworker();
	Error init() override;
	

private:
	SOCKET m_listenSocket;
	bool create_socket() override;//create listenSocket, connectSocket(for client), bind and start to listen
	bool create_connection() override; //accept client socket
};
