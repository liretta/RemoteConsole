#include "client_networker.h"

ClientNetworker::ClientNetworker()
{
	m_connectSocket = INVALID_SOCKET;

}

ClientNetworker::~ClientNetworker()
{
	closesocket(m_connectSocket);
	WSACleanup();
}

/*called private function for initialize library, create socket 
* and create connection
* if any of this function ended with error - return corresponding error 
*/

Error ClientNetworker::init()
{
	Error result = OK;
	if (init_library() == false)
	{
		result = ERR_LIBRARY_INIT;
		return result;
	}

	if (create_socket() == false)
	{
		result = ERR_CREATE_SOCKET;
		return result;
	}

	if (create_connection() == false)
	{
		result = ERR_CREATE_CONNECTION;
		return result;
	}
	
	return result;
};

/*initialized library for networking
* return true if initialize was successfull
*/
bool ClientNetworker::init_library()
{
	bool result = false;
	WORD DLLVersion = MAKEWORD(2, 1);
	result = WSAStartup(DLLVersion, &m_wsa); //return zero, if successfull
	return !result;
}

/*create socket for connection
* return true if socket was create successfull
*/
bool ClientNetworker::create_socket()
{
	bool result = false;
	int sizeAddr = sizeof(m_addr);
	m_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_addr.sin_port = htons(1111);
	m_addr.sin_family = AF_INET;

	m_connectSocket = socket(AF_INET, SOCK_STREAM, NULL);
	if (m_connectSocket != INVALID_SOCKET)
	{
		result = true;
	}
	else
	{
		result = false;
		WSACleanup();
	}

	return result;
}

/*create connection 
*return true if connection was create successfull
*/
bool ClientNetworker::create_connection()
{
	bool result = false;
	result = connect(m_connectSocket, (SOCKADDR*)&m_addr, sizeof(m_addr));
	if (result == false)
	{
		closesocket(m_connectSocket);
		m_connectSocket = INVALID_SOCKET;
	}
	return result;
}

/*sending string to server
* first of all send the size of string-message
* Size of this sending message is default = sizeofint
* then send message
* return true if sendindg was successfull
*/
bool ClientNetworker::send(std::string a_message)
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
};

/*revieved string from connection socket
* first of all recieved info with size of string-message
* size of this message is defaul = sizeofint
* then recieved string-message
* if recieved was sucessfull, return string from server
* otherwise, return string with error-message
*/
std::string ClientNetworker::receive()
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
		int byteRecieved = 0, tempByteRecieved=0;

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
bool ClientNetworker::shutdownSend() 
{
	int result = -1;
	result = _WINSOCKAPI_::shutdown(m_connectSocket, 1); //return zero if is successfull
	return !result;
}


/* stopped sending process for currens socket
* return true if shutdown was successfull
*/
bool ClientNetworker::shutdownRecieve()
{
	int result = -1;
	result = _WINSOCKAPI_::shutdown(m_connectSocket, 0); //return zero if is successfull
	return !result;
}

/* stopped sending process for currens socket
* return true if shutdown was successfull
*/
bool ClientNetworker::shutdownSendRecieve()
{
	int result = -1;
	result = _WINSOCKAPI_::shutdown(m_connectSocket, 2); //return zero if is successfull
	return !result;
}
