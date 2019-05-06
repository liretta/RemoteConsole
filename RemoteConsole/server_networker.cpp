#include "server_networker.h"

ServerNetworker::ServerNetworker()
{
	m_connectSocket = INVALID_SOCKET;
	m_listenSocket = INVALID_SOCKET;
}

ServerNetworker::~ServerNetworker()
{
	closesocket(m_connectSocket);
	closesocket(m_listenSocket);
	WSACleanup();
}

/*initialized library for networking
* return true if initialize was successfull
*/
bool ServerNetworker::init_library() 
{
	bool result = true;
	WORD DLLVersion = MAKEWORD(2, 1);
	result = WSAStartup(DLLVersion, &m_wsa); //return zero if it successfull
	return !result;
}

/*create listenSocket, connectSocket(for client), bind and start to listen
*return true if all socket was successfull created, binded and listened
*/
bool ServerNetworker::create_socket()
{
	bool result = true;
	int sizeAddr = sizeof(m_addr);
	m_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_addr.sin_port = htons(1111);
	m_addr.sin_family = AF_INET;

	m_listenSocket = socket(m_addr.sin_family, SOCK_STREAM, NULL);
	if (m_listenSocket == INVALID_SOCKET)
	{
		WSACleanup();
		result = false;
		return result;
	}

	result = bind(m_listenSocket, (sockaddr*)&m_addr, sizeof(m_addr));
	if (result == SOCKET_ERROR)
	{
		result = false;
		closesocket(m_listenSocket);
		WSACleanup();
		return result;
	}

	result = listen(m_listenSocket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		result = false;
		closesocket(m_listenSocket);
		WSACleanup();
		return result;
	}

	return result;
}

/*accept client socket
* return true if connection was create successfull
*/
bool ServerNetworker::create_connection()
{
	int result = true;
	int addrSize = sizeof(m_addr);
	m_connectSocket = accept(m_listenSocket, (sockaddr*)&m_addr, &addrSize);
	if (m_connectSocket == INVALID_SOCKET)
	{
		result = false;
		closesocket(m_listenSocket);
		WSACleanup();
	}

	return result;
}

/*called private function for initialize library, create socket for client and listening
* create connection
* if any of this function ended with error - return corresponding error
*/
Error ServerNetworker::init()
{
	Error result = OK;
	if (init_library() == false)
	{
		result = ERR_LIBRARY_INIT;
		return result;
	}
	else
	{
		if (create_socket() == false)
		{
			result = ERR_CREATE_SOCKET;
			return result;
		}
		else
		{
			if (create_connection() == false)
			{
				result = ERR_CREATE_CONNECTION;
				return result;
			}
		}
	}
	return result;
}

bool send(std::string a_message);
std::string receive();
bool shutdownSend();
bool shutdownRecieve();
bool shutdownSendRecieve();

