/*!
 * interface class for all functionality in the server side
 */
#pragma once
#include "server_cryptor.h"
#include "server_executor.h"
#include "server_logger.h"
#include "server_networker.h"
#include "marshaller.h"

class IServer
{
public:
	virtual ServerExecutor& getExecutor() = 0;
	virtual ServerLogger& getLogger() = 0;
	virtual ServerNetworker& getNetworker() = 0;
	virtual void run() = 0;
};
