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

/*sending string to client
* first of all send the size of string-message
* Size of this sending message is default = sizeofint
* then send message
* return true if sendindg was successfull
*/
bool ServerNetworker::send(std::string a_message)
{
	int result = SOCKET_ERROR;
	union
	{
		char cSize[4];
		int iSize;
	} uMessageSize;

	uMessageSize.iSize = a_message.size();

	//send message-string size
	result = _WINSOCKAPI_::send(m_connectSocket, uMessageSize.cSize, sizeof(uMessageSize), 0);

	if (result == SOCKET_ERROR)
	{
		return false;
	}
	else
	{
		//send message-string
		result = _WINSOCKAPI_::send(m_connectSocket, a_message.c_str(), uMessageSize.iSize, 0);
	}

	if (result == SOCKET_ERROR)
	{
		return false;
	}
	else
	{
		return true;
	}

}
std::string ServerNetworker::receive()
{
	std::string strBuff;
	union
	{
		char cSize[4];
		int iSize;
	} uMessageSize;
	int result = -1;

	//recieve message size
	result = recv(m_connectSocket, uMessageSize.cSize, sizeof(uMessageSize), 0);

	if (result == SOCKET_ERROR)
	{
		strBuff = "";
		return strBuff;
	}
	else
	{
		//recieve string-message
		std::vector<char> vBuff(MAX_BUFF_LEN);
		int byteRecieved = 0, tempByteRecieved = 0;

		while (byteRecieved < uMessageSize.iSize)
		{
			tempByteRecieved = recv(m_connectSocket, &vBuff[0], uMessageSize.iSize, 0);
			if (tempByteRecieved == -1)
			{
				strBuff = "";
				return strBuff;
			}
			else
			{
				strBuff.append(vBuff.cbegin(), vBuff.cend());
			}
			byteRecieved += tempByteRecieved;
		}
		return strBuff;
	}
}

/* stopped sending process for currens socket
* return true if shutdown was successfull
*/
bool ServerNetworker::shutdownSend()
{
	int result = -1;
	result = _WINSOCKAPI_::shutdown(m_connectSocket, 1); //return zero if is successfull
	return !result;
}


/* stopped sending process for currens socket
* return true if shutdown was successfull
*/
bool ServerNetworker::shutdownRecieve()
{
	int result = -1;
	result = _WINSOCKAPI_::shutdown(m_connectSocket, 0); //return zero if is successfull
	return !result;
}

/* stopped sending process for currens socket
* return true if shutdown was successfull
*/
bool ServerNetworker::shutdownSendRecieve()
{
	int result = -1;
	result = _WINSOCKAPI_::shutdown(m_connectSocket, 2); //return zero if is successfull
	return !result;
}
