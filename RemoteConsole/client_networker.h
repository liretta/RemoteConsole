/*! 
 * create connection in the client side
 */
#pragma once
#include "base_networker.h"

class ClientNetworker : public BaseNetworker
{
public:
	ClientNetworker() = default;
	~ClientNetworker() = default;
	Error init(const std::string &def_adr = "127.0.0.1") override; //called init_library, create_socked and create_connection functions. Return corresponding error if some of them ended with error
	
private:
	bool create_socket(const std::string &def_adr) override;
	bool create_connection() override; 
};
