#pragma once
#include "base_networker.h"

//class INetworker;

class ClientNetworker : public BaseNetworker
{
public:
	ClientNetworker();
	~ClientNetworker();
	Error init(); //called init_library, create_socked and create_connection functions. Return corresponding error if some of them ended with error
	
private:
	bool create_socket();
	bool create_connection(); 
};
