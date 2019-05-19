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
	virtual Error init(const std::string &def_adr = "127.0.0.1") = 0;
	virtual bool send(const std::string &a_message)= 0;
	virtual std::string receive() = 0;
	virtual bool shutdownSend() = 0;
	virtual bool shutdownReceive() = 0;
	virtual bool shutdownSendReceive() = 0;

protected:
	virtual bool init_library() = 0;
	virtual bool create_socket(const std::string &def_adr) = 0;
	virtual bool create_connection() = 0;
};
