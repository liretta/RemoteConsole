#pragma once
#include "base_networker.h"

class ClientNetworker : public BaseNetworker
{
public:
	ClientNetworker() = default;
	~ClientNetworker() = default;
	Error init() override; //called init_library, create_socked and create_connection functions. Return corresponding error if some of them ended with error
	
private:
	bool create_socket() override;
	bool create_connection() override; 
};
