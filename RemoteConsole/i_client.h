#pragma once
#include "client_cryptor.h"
#include "client_networker.h"
#include "client_logger.h"
#include "client_executor.h"

class ClientLogger;
class ClientExecutor;
class ClientCryptor;
class ClientNetworker;

class IClient
{
public:
	virtual ClientExecutor& getExecutor()  = 0;
	virtual ClientLogger& getLogger()  = 0;
	virtual ClientNetworker& getNetworker()  = 0;
};
