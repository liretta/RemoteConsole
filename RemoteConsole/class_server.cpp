#include "class_server.h"

Server::Server():m_logger(m_networker)
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


void Server::run()
{
	//init networker
	Error er = m_networker.init();
	PrintError(er);
	if (er != OK)
	{
		return;
	}
	

	while(1)
	{
		bool log_in_result = false;
		bool is_connection = true;
		do 
		{
			log_in_result = client_log_in(is_connection);
		} while (!log_in_result && is_connection);
		/*while(!log_in_result)
		{
			log_in_result = client_log_in();
		}*/

		
		while(is_connection)
		{
			is_connection = data_exchange();
			
		}
	}
}


bool Server::client_log_in(bool &is_connection)
{
	bool result = false;
	
	std::string str = m_networker.receive();
	if (str == "#Error" or str == "") //here will be checking ERR result. Save it. If trable with connection - again check pass, else - again listen
	{
		is_connection = false;
		return false;
	}
	std::pair<std::wstring, std::wstring> wlog = Marshaller::unpackAuthorizationData(STRINGtoWSTRING(str));
	std::pair<std::string, std::string> str_log = std::make_pair(WSTRINGtoSTRING(wlog.first), WSTRINGtoSTRING(wlog.second));

	//ServerLogger slog(m_networker);
	result = m_logger.check_password(str_log, USER);
	m_networker.send(WSTRINGtoSTRING(Marshaller::packResult(result)));

	return result;
}

bool Server::data_exchange()
{
	std::string str = m_networker.receive();
	if (str == "#Error") //here will be checking ERR result. Save it. If trable with connection - again check pass, else - again listen
	{
		return false;
	}
	std::wstring w_mess = STRINGtoWSTRING(str);
	std::wstring comm = Marshaller::unpackMessage(Marshaller::getMode(w_mess), w_mess);

	//ServerExecutor s_exc;
	m_executor.initialize();
	bool result = m_executor.execute(comm);
	if (result)
	{
		std::wstring result_mess = m_executor.getResult();
		std::wstring send_mess = Marshaller::packMessage(Marshaller::Type::Command, result_mess);
		m_networker.send(WSTRINGtoSTRING(send_mess));
	}
	else
	{
		m_networker.send(WSTRINGtoSTRING(Marshaller::packResult(result)));
	}
	return true;
}