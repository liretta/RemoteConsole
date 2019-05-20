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
	INetworker() = default;
	virtual ~INetworker() = default;
	virtual Error init() = 0;
	virtual bool send(std::string a_message)= 0;
	virtual std::string receive() = 0;
	virtual bool shutdownSend() = 0;
	virtual bool shutdownReceive() = 0;
	virtual bool shutdownSendReceive() = 0;

protected:
	virtual bool init_library() = 0;
	virtual bool create_socket() = 0;
	virtual bool create_connection() = 0;
};
