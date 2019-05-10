#include "base_networker.h"

BaseNetworker::BaseNetworker() 
{
	m_connectSocket = INVALID_SOCKET;
	ZeroMemory(&m_addr, sizeof(m_addr));
};

/*initialized library for networking
* return true if initialize was successfull
*/
bool BaseNetworker::init_library()
{
	bool result = true;
	WORD DLLVersion = MAKEWORD(2, 1);
	result = WSAStartup(DLLVersion, &m_wsa); //return zero if it successfull
	return !result;
}

/*sending string through connection socket
* first of all send the size of string-message
* size of this sending message is default = sizeofint
* then send string message
* return true if sendindg was successfull
*/
bool BaseNetworker::send(std::string a_message)
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
	
	return true;
}

/*receive string from connection socket
* first of all receiving the size of string-message
* size of this sending message is default = sizeofint
* then receive string message
* return received string if sendindg was successfull
* return string "#Error" in another case
*/
std::string BaseNetworker::receive()
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
		strBuff = "#Error";
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
				strBuff = "#Error";
				return strBuff;
			}
			else
			{
				strBuff.append(vBuff.cbegin(), vBuff.cbegin()+ uMessageSize.iSize);
			}
			byteRecieved += tempByteRecieved;
		}
		return strBuff;
	}
}


/* stopped sending process for currens socket
* return true if shutdown was successfull
*/
bool BaseNetworker::shutdownSend()
{
	int result = -1;
	result = _WINSOCKAPI_::shutdown(m_connectSocket, 1); //return zero if is successfull
	return !result;
}


/* stopped sending process for currens socket
* return true if shutdown was successfull
*/
bool BaseNetworker::shutdownRecieve()
{
	int result = -1;
	result = _WINSOCKAPI_::shutdown(m_connectSocket, 0); //return zero if is successfull
	return !result;
}

/* stopped sending process for currens socket
* return true if shutdown was successfull
*/
bool BaseNetworker::shutdownSendRecieve()
{
	int result = -1;
	result = _WINSOCKAPI_::shutdown(m_connectSocket, 2); //return zero if is successfull
	return !result;
}
