#include "class_server.h"

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


void Server::Run()
{
	//init networker
	Error er = m_networker.init();
	if (er != OK)
	{
		PrintError(er);
		return;
	}
	
	//logIn client
	//result = false
	//while (!result)
	//{
	//	if(check client socket = true) - maybe check result send/recv to WSAETIMEDOUT or WSAECONNRESET??  and write it to some variable inside networker?
	//result = client_log_in()
	// else
	//re_asset listen socket. Change quantity of listen
	// break;
	//}
	
	//if result
	//exchange data with client while check_client_socket is true

}


bool Server::client_log_in()
{
	bool result = false;
	
	std::string str = m_networker.receive();
	std::pair<std::wstring, std::wstring> wlog = Marshaller::unpackAuthorizationData(STRINGtoWSTRING(str));
	std::pair<std::string, std::string> str_log = std::make_pair(WSTRINGtoSTRING(wlog.first), WSTRINGtoSTRING(wlog.second));

	ServerLogger slog(m_networker);
	result = slog.check_password(str_log, USER);
	m_networker.send(WSTRINGtoSTRING(Marshaller::packResult(result)));

	return result;
}

