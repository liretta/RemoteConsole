#include "server_networker.h"

ServerNetworker::~ServerNetworker()
{
	if(m_listen_socket!=INVALID_SOCKET)
	{
		closesocket(m_listen_socket);
	}
}

/*!
 * create listenSocket, connectSocket(for client), bind and start to listen
 * @return true if all socket was successful created, binded and listened
 */
bool ServerNetworker::create_socket(const std::string &def_adr)
{
	bool bResult = true;
	int iResult = -1;
	int sizeAddr = sizeof(m_addr);
	m_addr.sin_addr.s_addr = inet_addr(def_adr.c_str());
	m_addr.sin_port = htons(1111);
	m_addr.sin_family = AF_INET;

	m_listen_socket = socket(m_addr.sin_family, SOCK_STREAM, NULL);
	if (m_listen_socket == INVALID_SOCKET)
	{
		bResult = false;
		return bResult;
	}

	iResult = bind(m_listen_socket, (sockaddr*)&m_addr, sizeof(m_addr));
	if (iResult == SOCKET_ERROR)
	{
		bResult = false;
		return bResult;
	}

	iResult = listen(m_listen_socket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		bResult = false;
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
	m_connect_socket = accept(m_listen_socket, (sockaddr*)&m_addr, &addrSize);
	if (m_connect_socket == INVALID_SOCKET)
	{
		result = false;
	}
	return result;
}

/*! 
 * called private function for initialize library, create socket for client and listening
 * create connection
 * @return corresponding error if any of this function ended with error
 */
Error ServerNetworker::init(const std::string &def_adr)
{
	Error result = OK;
	if (init_library() == false)
	{
		result = ERR_LIBRARY_INIT;
		return result;
	}
	else
	{
		if (create_socket(def_adr) == false)
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
