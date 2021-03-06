/*! 
 * base abstract class for networking
 */
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
	virtual ~BaseNetworker();
	bool send(const std::vector<char> &a_message) override; //send message through connection socket
	bool receive(std::vector<char> &a_message) override; //receive message from connection socket. Save result to a_message
	virtual Error init(const std::string &def_adr = "127.0.0.1") override = 0;
	bool shutdownSend() override; //shutdown network object for sending
	bool shutdownReceive() override; //shutdown network object for receiving
	bool shutdownSendReceive() override; //shutdown network object for sending and receiving

protected:
	SOCKET m_connect_socket; //socket for connection
	WSAData m_wsa; //library data
	SOCKADDR_IN m_addr; //structure with address, protocol and other data for creating connection

	bool init_library() override; //initialize library
};

