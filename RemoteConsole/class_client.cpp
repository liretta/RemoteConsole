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

/*!
 * receive from the server package with the pure public key
 * give the key to m_cryptor
 * @return false if received was failed or the key wasn't validate 
 */
bool Client::setCryptoKey()
{
	std::vector<char> recv_message;
	if (!m_networker.receive(recv_message))
		return false;

	m_cryptor.setKey(recv_message);
	return true;
}


/*!
 * m_cryptor generate the synchronous key and init_vector for synchronous key
 * pack their into the vector<char> format
 * send vector<char> into server
 * @return true if key generation and sending were successes, return false if if was problem with receiving or key generation 
 */
bool Client::sendCryptoKey()
{
	if(!m_cryptor.generateKey())
	{
		return false;
	}

	if(!m_networker.send(m_cryptor.keyEncrypt()))
	{
		return false;
	}

	if (!m_networker.send(m_cryptor.ivEncrypt()))
	{
		return false;
	}
	return true;
}

