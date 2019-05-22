#include "class_client.h"

ClientExecutor& Client::getExecutor() 
{
	return m_executor;
};

ClientLogger& Client::getLogger()  
{
	return m_logger;
}

ClientNetworker& Client::getNetworker()
{
	return m_networker;
}

