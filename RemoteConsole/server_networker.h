#pragma once
#include "base_networker.h"
//TODO: do it multi-thread - create vector with sockets and create new thread when new cliend is connecting
//bring serd/recieve and shutdown-methods to base class

class BaseNetworker;

class ServerNetworker : public BaseNetworker
{
public:
	ServerNetworker();
	~ServerNetworker();
	Error init();
	bool send(std::string a_message);
	std::string receive();
	bool shutdownSend();
	bool shutdownRecieve();
	bool shutdownSendRecieve();

private:
	WSAData m_wsa;
	SOCKET m_connectSocket;//socket for client
	SOCKET m_listenSocket;
	SOCKADDR_IN m_addr;
	bool init_library(); //initinialize the library
	bool create_socket();//create listenSocket, connectSocket(for client), bind and start to listen
	bool create_connection(); //accept client socket
};
