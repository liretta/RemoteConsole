#include "base_networker.h"

BaseNetworker::BaseNetworker(): m_connect_socket(INVALID_SOCKET)
{
	ZeroMemory(&m_addr, sizeof(m_addr));
	ZeroMemory(&m_wsa, sizeof(m_wsa));
};

BaseNetworker::~BaseNetworker()
{
	if(m_connect_socket!= INVALID_SOCKET)
	{
		closesocket(m_connect_socket);
	}
	WSACleanup();
}

/*!
 * initialized library for networking
 * @return true if initialize was successful
 */
bool BaseNetworker::init_library()
{
	bool result = true;
	WORD DLLVersion = MAKEWORD(2, 1);
	result = WSAStartup(DLLVersion, &m_wsa); //return zero if it successful
	return !result;
}

/*!
 * sending encrypting data through connection socket
 * first of all send the size of message
 * size of this sending message is default = sizeofint
 * then send message
 * @return true if sending was successful
 * @return false if not all data was send or connection have been lost
 */
bool BaseNetworker::send(const std::vector<char> &a_message)
{
	int result = SOCKET_ERROR, 
		m_size = a_message.size(), 
		int_size = sizeof(int),
		temp_byte_send = 0, //stores temporarily the number of bytes transferred and the value returned by the winsock's function "send"
		byte_send = 0;

	//try to send size of message 
	while(byte_send<int_size)
	{
		temp_byte_send = ::send(m_connect_socket, (char*)&m_size+byte_send, int_size-byte_send, 0);
		if (temp_byte_send == SOCKET_ERROR)
		{
			std::wcerr << L"Error sending data " << WSAGetLastError() << std::endl;
			return false;
		}
		byte_send += temp_byte_send;
	}

	//try to send message
	byte_send = 0, temp_byte_send = 0;
	while (byte_send < m_size)
	{
		temp_byte_send = ::send(m_connect_socket, &a_message[0]+byte_send, m_size-byte_send, 0);
		if (temp_byte_send == SOCKET_ERROR)
		{
			std::wcerr << L"Error sending data " << WSAGetLastError() << std::endl;
			return false;
		}
		byte_send += temp_byte_send;
	}
	return true;
}

/*!
 * receive encrypted data from connection socket
 * first of all receiving the size of the message
 * size of this first receiving message is default = sizeofint
 * then receive message
 * save receiving message in a_message by reference
 * @return true if receiving was successful
 * @return false if not all data was receiving or connection have been lost
 * closed connection socket if connection have been lost
 */
bool BaseNetworker::receive(std::vector<char>& a_message)
{
	int m_size = 0, byte_received = 0, temp_byte_received = 0;

	//receive message size
	while(byte_received < sizeof(int))
	{
		temp_byte_received = recv(m_connect_socket, (char*)&m_size+byte_received, sizeof(int)-byte_received, 0);
		if (temp_byte_received == SOCKET_ERROR || temp_byte_received == 0)
		{
			std::wcerr << L"Error receiving data " << WSAGetLastError() << std::endl;
			closesocket(m_connect_socket);
			//create_connection();
			return false;
		}

		byte_received += temp_byte_received;
	}
	
	//receive string-message
	a_message.resize(m_size);
	byte_received = 0, temp_byte_received = 0;
	
	//read message in parts
	while (byte_received < m_size)
	{
		temp_byte_received = recv(m_connect_socket, &a_message[0]+byte_received, m_size-byte_received, 0);
		if (temp_byte_received == SOCKET_ERROR || temp_byte_received == 0)
		{
			closesocket(m_connect_socket);
			//create_connection();
			return false;
		}
		
		byte_received += temp_byte_received;
	}
	return true;
}


/*!
 * stopped sending process for current socket
 * @return true if shutdown was successful
 */
bool BaseNetworker::shutdownSend()
{
	return !(::shutdown(m_connect_socket, 1)); //return zero if is successful
}

/*!
 * stopped sending process for current socket
 * @return true if shutdown was successful
 */
bool BaseNetworker::shutdownReceive()
{
	return !(::shutdown(m_connect_socket, 0)); //return zero if is successful
}

/*!
 * stopped sending process for current socket
 * @return true if shutdown was successful
 */
bool BaseNetworker::shutdownSendReceive()
{
	return !(::shutdown(m_connect_socket, 2)); //return zero if is successful
}
