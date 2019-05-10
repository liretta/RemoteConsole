/* interface class for networker
*
*/
#pragma once
#pragma comment (lib, "Ws2_32.lib")

#include <winsock.h>
#include <string>
#include "error_list.h"

class INetworker
{
public:
	INetworker() {};
	virtual ~INetworker() {};
	virtual Error init() = 0;
	virtual bool send(std::string a_message)= 0;
	virtual std::string receive() = 0;
	virtual bool shutdownSend() = 0;
	virtual bool shutdownRecieve() = 0;
	virtual bool shutdownSendRecieve() = 0;

protected:
	virtual bool init_library() = 0;
	virtual bool create_socket() = 0;
	virtual bool create_connection() = 0;
};
