#pragma once
#pragma comment (lib, "Ws2_32.lib")

#include <winsock.h>
#include <string>
#include <vector>
#include "error_list.h"
static const unsigned int MAX_BUFF_LEN = 4096;


class BaseNetworker
{
public:
	BaseNetworker() {};
	virtual ~BaseNetworker() {};
	virtual Error init() = 0;
	virtual bool send(std::string a_message)= 0;
	virtual std::string receive() = 0;
	virtual bool shutdownSend() = 0;
	virtual bool shutdownRecieve() = 0;
	virtual bool shutdownSendRecieve() = 0;

private:
	WSAData m_wsa;
	SOCKET m_connectSocket;
	SOCKADDR_IN m_addr;

	virtual bool init_library() = 0;
	virtual bool create_socket() = 0;
	virtual bool create_connection() = 0;
};
