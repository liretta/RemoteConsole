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
 * here is an endless loop with creation connection with client, checking his logIn information and data exchange within server and client
 * work in order:
 * 1. initialize networker, listen for the client
 * 2. checking logIn data while it wasn't successfully 
 * 3. Running an endless loop with data exchange while connection is presents
 * if connection lost, start all loop again
 */
void Server::run()
{
	while (is_connection)
	{
		is_connection = data_exchange();
	}
}

void Server::logIn()
{
	bool result_by_log_in = false;
	//check logIn data while connection is present and log+pass wasn't connect
	do
	{
		result_by_log_in = client_log_in();
	} while (!result_by_log_in && is_connection);
}

/*!
 * receiving logIn data from client, check it and return result
 * if it is problem with receiving, change is_connection value
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

	std::wstring comm = 
		Marshaller::unpackMessage(Marshaller::getMode(m_cryptor.decrypt(tmp_vc)), m_cryptor.decrypt(tmp_vc));
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

bool Server::reconnect()
{
	is_connection = m_networker.create_connection();
	return is_connection;
}