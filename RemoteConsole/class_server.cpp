#include "class_server.h"

Server::Server():m_logger(m_networker), is_connection(false)
{}

ServerExecutor& Server::getExecutor()
{
	return m_executor;
};

ServerLogger& Server::getLogger()
{
	return m_logger;
}

ServerNetworker& Server::getNetworker()
{
	return m_networker;
}

/*!
 * here is loop with exchange data between client ad server while connection is present
 * work in order:
 * if connection lost, return control
 */
void Server::run()
{
	while (is_connection)
	{
		is_connection = data_exchange();
	}
}

/*!
* call function with checking authorization data while connection is present and while log + pass won't connect
* if connection lost, return control
*/
void Server::logIn()
{
	bool result_by_log_in = false;
	do
	{
		result_by_log_in = client_log_in();
	} while (!result_by_log_in && is_connection);
}

/*!
 * receiving authorization data from client
 * unpack, decrypt it and check data
 * return packed and encrypted result of checking
 * if was problem with receiving, change is_connection value
 * @return true if log+pass is correct
 * @return false in another case (wrong log/pass, cannot open file with login data or connection was lost)
 */
bool Server::client_log_in()
{
	bool result = false;
	
	std::vector<char> tmp_vc;
	result = m_networker.receive(tmp_vc);
	if (!result) 
	{
		is_connection = false;
		PrintError(ERR_CONNECTION_LOST);
		return result;
	}

	std::pair<std::wstring, std::wstring> wlog = Marshaller::unpackAuthorizationData(m_cryptor.decrypt(tmp_vc));
	std::pair<std::string, std::string> str_log = std::make_pair(WSTRINGtoSTRING(wlog.first), WSTRINGtoSTRING(wlog.second));

	result = m_logger.check_password(str_log);

	m_networker.send(m_cryptor.encrypt( (Marshaller::packResult(result) ) ));

	return result;
}

/*!
 * function for endless exchange data with server and client
 * decrypt and unpack received data from connection socket, execute command, encrypt and pack result
 * @return true if receiving/sending was successful
 * @return false if connection have been lost or was any other error with encryptor or marshaller
 */
bool Server::data_exchange()
{
	std::vector<char> tmp_vc;
	bool result = m_networker.receive(tmp_vc);
	if (!result)
	{
		PrintError(ERR_CONNECTION_LOST);
		return result;
	}

	std::wstring decrypt_message = m_cryptor.decrypt(tmp_vc);
	std::wstring comm = 
		Marshaller::unpackMessage(Marshaller::getMode(decrypt_message), decrypt_message);
    std::wcout << comm << std::endl;

	m_executor.initialize();
	result = m_executor.execute(comm);
	if (result)
	{
		std::wstring result_mess = m_executor.getResult();
		std::wstring send_mess = Marshaller::packMessage(Marshaller::Type::Command, result_mess);
		m_networker.send(m_cryptor.encrypt(send_mess));
	}
	else
	{
		m_networker.send(m_cryptor.encrypt(Marshaller::packResult(result)));
	}
	return true;
}

/*!
* first initialization networker and waiting for first connection client
*/
bool Server::waitingForConnection()
{
    //init networker
    Error er = m_networker.init();
    PrintError(er);
    if (er != OK)
    {
		is_connection = false;
        return is_connection;
    }
	is_connection = true;
	return is_connection;
}

/*!
* create new connection when the first client was disconnected or connection have been lost in another reasons
*/
bool Server::reconnect()
{
	is_connection = m_networker.create_connection();
	return is_connection;
}

/*!
 * generate pair: public & private keys
 * pack private key and send it to client
 * @return false if keys generation was failed or sending ended with error
 */
bool Server::sendKey()
{
	if (!m_cryptor.generateKey())
	{
		return false;
	}

	if(!m_networker.send(m_cryptor.getPublicKey()))
	{
		is_connection = false;
		return false;
	}
	return true;
}


/*! 
 * received synchronous key and initialization vector for him from client
 * take this data to m_cryptor
 */
bool Server::getKey()
{
	std::vector<char> tmp_key, tmp_iv;
	if(!m_networker.receive(tmp_key))
	{
		is_connection = false;
		return false;
	}
	if(!m_networker.receive(tmp_iv))
	{
		is_connection = false;
		return false;
	}

	return m_cryptor.setKey(tmp_key, tmp_iv);
}

