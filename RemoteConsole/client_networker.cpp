#include "client_networker.h"
/*!
 * called private function for initialize library, create socket 
 * and create connection
 * @return corresponding error if any of this function ended with error 
 * @return OK if initialize was successful
 */
Error ClientNetworker::init(const std::string &def_adr)
{
	Error result = OK;
	if (init_library() == false)
	{
		result = ERR_LIBRARY_INIT;
		return result;
	}

	if (create_socket(def_adr) == false)
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
/*!
 * create socket for connection
 * @return true if socket created successful
 */
bool ClientNetworker::create_socket(const std::string &def_adr)
{
	bool result = false;
	int sizeAddr = sizeof(m_addr);
	m_addr.sin_addr.s_addr = inet_addr(def_adr.c_str());
	m_addr.sin_port = htons(1111);
	m_addr.sin_family = AF_INET;

	m_connect_socket = socket(AF_INET, SOCK_STREAM, NULL);
	if (m_connect_socket != INVALID_SOCKET)
	{
		result = true;
	}
	else
	{
		result = false;
	}
	return result;
}

/*!
 * create connection 
 * @return true if connection was create successful
 */
bool ClientNetworker::create_connection()
{
	bool result = false;
	result = connect(m_connect_socket, (SOCKADDR*)&m_addr, sizeof(m_addr)); //winsock function return zero if successful
	return !result;
}
