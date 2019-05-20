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
 * sending string through connection socket
 * first of all send the size of string-message
 * size of this sending message is default = sizeofint
 * then send string message
 * @return true if sending was successful
 */
bool BaseNetworker::send(const std::string &a_message)
{
	int result = SOCKET_ERROR;
	union
	{
		char c_size[4];
		int i_size;
	} u_message_size;
	u_message_size.i_size = a_message.size();

	//send size of message
	result = ::send(m_connect_socket, u_message_size.c_size, sizeof(u_message_size), 0);
	if (result == SOCKET_ERROR)
	{
		return false;
	}
	else
	{
		//send message string
		result = ::send(m_connect_socket, a_message.c_str(), u_message_size.i_size, 0);
	}

	if (result == SOCKET_ERROR)
	{
		return false;
	}
	
	return true;
}

/*!
 * receive string from connection socket
 * first of all receiving the size of string-message
 * size of this sending message is default = sizeofint
 * then receive message
 * @return received string if sending was successful
 * @return string "#Error" in another case
 */
std::string BaseNetworker::receive()
{
	std::string str_buff;
	union
	{
		char c_size[4];
		int i_size;
	} u_message_size;
	u_message_size.i_size = 0;
	
	//receive message size
	int result = recv(m_connect_socket, u_message_size.c_size, sizeof(u_message_size), 0);

	if (result == SOCKET_ERROR)
	{
		str_buff = "#Error";
		return str_buff;
	}
	else
	{
		//receive string-message
		std::vector<char> v_buff(u_message_size.i_size);
		int byte_received = 0, temp_byte_received = 0;

		//read message in parts
		while (byte_received < u_message_size.i_size)
		{
			temp_byte_received = recv(m_connect_socket, &v_buff[0]+byte_received, u_message_size.i_size - byte_received, 0);
			if (temp_byte_received == SOCKET_ERROR)
			{
				str_buff = "#Error";
				return str_buff;
			}
			else
			{
				byte_received += temp_byte_received;
			}	
		}

		if (byte_received == u_message_size.i_size)
		{
			str_buff.append(v_buff.cbegin(), v_buff.cbegin() + u_message_size.i_size);
		}

		return str_buff;
	}
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
