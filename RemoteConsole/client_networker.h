#pragma once
#include "base_networker.h"

class ClientNetworker : public BaseNetworker
{
public:
	ClientNetworker();
	~ClientNetworker();
	Error init(); //called init_library, create_socked and create_connection functions. Return corresponding error if some of them ended with error
	bool send(std::string a_message);  //called send-function from winsock to string-message to server
	std::string receive(); //recieved string-message from server
	bool shutdownSend(); //lock socket for sending. Return true if successfull
	bool shutdownRecieve(); //lock socket for recieving. Return true if successfull
	bool shutdownSendRecieve(); //lock socket for sending and recieving. Return true if successfull

private:
	WSAData m_wsa;
	SOCKET m_connectSocket;
	SOCKADDR_IN m_addr;
	bool init_library();
	bool create_socket();
	bool create_connection(); 
};
