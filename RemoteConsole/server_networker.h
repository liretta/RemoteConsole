#pragma once
#include "base_networker.h"
//TODO: do it multi-thread - create vector with sockets and create new thread when new cliend is connecting
//bring serd/recieve and shutdown-methods to base class

//class INetworker;

class ServerNetworker : public BaseNetworker
{
public:
	ServerNetworker();
	~ServerNetworker();
	Error init();
	

private:
	SOCKET m_listenSocket;
	//bool init_library(); //initinialize the library
	bool create_socket();//create listenSocket, connectSocket(for client), bind and start to listen
	bool create_connection(); //accept client socket
};
