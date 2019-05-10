#pragma once
#pragma comment (lib, "Ws2_32.lib")
#include "i_networker.h"
#include "error_list.h"

#include <winsock.h>
#include <string>
#include <vector>

static const unsigned int MAX_BUFF_LEN = 4096;


class BaseNetworker: public INetworker
{
public:
	BaseNetworker();
	virtual ~BaseNetworker() {};
	bool send(std::string a_message); //send message through connection socket
	std::string receive(); //receive message from connection socket
	virtual Error init() = 0;
	bool shutdownSend(); //shutdown network object for sending
	bool shutdownRecieve(); //shutdown network object for receiving
	bool shutdownSendRecieve(); //shutdown network object for sending and receiving

protected:
	WSAData m_wsa; //library data
	SOCKET m_connectSocket; //socket for connection
	SOCKADDR_IN m_addr; //structure with adress, protocol and other data for creating connection

	bool init_library(); //initialize library
	virtual bool create_socket() = 0; //create socket for connection
	virtual bool create_connection() = 0; //create connection with socket
};

