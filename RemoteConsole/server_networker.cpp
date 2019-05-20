#include "server_networker.h"

ServerNetworker::ServerNetworker()
{
	m_listenSocket = INVALID_SOCKET;
}

ServerNetworker::~ServerNetworker()
{
	closesocket(m_listenSocket);
}

/*!
 * create listenSocket, connectSocket(for client), bind and start to listen
 * @return true if all socket was successful created, binded and listened
 */
bool ServerNetworker::create_socket()
{
	bool bResult = true;
	int iResult = -1;
	int sizeAddr = sizeof(m_addr);
	m_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_addr.sin_port = htons(1111);
	m_addr.sin_family = AF_INET;

	m_listenSocket = socket(m_addr.sin_family, SOCK_STREAM, NULL);
	if (m_listenSocket == INVALID_SOCKET)
	{
		WSACleanup();
		bResult = false;
		return bResult;
	}

	iResult = bind(m_listenSocket, (sockaddr*)&m_addr, sizeof(m_addr));
	if (iResult == SOCKET_ERROR)
	{
		bResult = false;
		closesocket(m_listenSocket);
		WSACleanup();
		return bResult;
	}

	iResult = listen(m_listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		bResult = false;
		closesocket(m_listenSocket);
		WSACleanup(); 
		return bResult;
	}

	return bResult;
}

/*!
 * accept client socket
 * @return true if connection was create successful
 */
bool ServerNetworker::create_connection()
{
	int result = true;
	int addrSize = sizeof(m_addr);
	m_connect_socket = accept(m_listenSocket, (sockaddr*)&m_addr, &addrSize);
	if (m_connect_socket == INVALID_SOCKET)
	{
		result = false;
		closesocket(m_listenSocket);
		WSACleanup();
	}

	return result;
}

/*! 
 * called private function for initialize library, create socket for client and listening
 * create connection
 * @return corresponding error if any of this function ended with error
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
