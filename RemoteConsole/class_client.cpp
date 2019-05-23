#include "class_client.h"

Client::Client() : m_executor(m_networker, m_cryptor), m_logger(m_networker, m_cryptor)
{}

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

