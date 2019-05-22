/*!
 * temp test for checking networker
 * delete it after create unit-testing
 */
#pragma once
#include <iostream>
#include "error_list.h"
#include "server_networker.h"
#include "client_networker.h"
#include "client_logger.h"
#include "server_logger.h"
#include "client_executor.h"
#include "server_executor.h"



void TestNetworkerM()
{
	char choice = 'e';
	std::cout << "Please, choice you role: s(server) or c(client):";
	std::cin >> choice;
	
	switch (choice)
	{
	case 's':
	{
		ServerNetworker sn;
		Error er = sn.init();
		if (er != OK)
		{
			PrintError(er);
			break;
		}
		else
		{
			std::cout << "Connection created\n";
			//check login
			{
				bool result = false;

				
					std::string str = sn.receive();
					std::pair<std::wstring, std::wstring> wlog = Marshaller::unpackAuthorizationData(STRINGtoWSTRING(str));
					std::pair<std::string, std::string> str_log = std::make_pair(WSTRINGtoSTRING(wlog.first), WSTRINGtoSTRING(wlog.second));
					
					ServerLogger slog(sn);
					result = slog.check_password(str_log, USER);
					sn.send(WSTRINGtoSTRING(Marshaller::packResult(result)));				
				

				while (1)
				{
					std::string str = sn.receive();
					std::wstring w_mess = STRINGtoWSTRING(str);
					std::wstring comm = Marshaller::unpackMessage(Marshaller::getMode(w_mess), w_mess);

					ServerExecutor s_exc;
					s_exc.initialize();
					bool result = s_exc.execute(comm);
					if (result)
					{
						std::wstring result_mess = s_exc.getResult();
						std::wstring send_mess = Marshaller::packMessage(Marshaller::Type::Command, result_mess);
						sn.send(WSTRINGtoSTRING(send_mess));
					}
					else
					{
						sn.send(WSTRINGtoSTRING(Marshaller::packResult(result)));
					}
				}
			}
		}
	}
	break;

	case 'c':
	{
		ClientNetworker cn;
		Error er = cn.init();
		if (er != OK)
		{
			PrintError(er);
			break;
		}
		else
		{
			std::cout << "Connection created\n";
			std::string log, pas;
			std::cout << "Login: ";
			std::cin >> log;
			std::cout << "Pass: ";
			std::cin >> pas;

			ClientLogger logger(cn);
			if (!logger.check_password(std::make_pair(log, pas), USER))
			{
				std::cout << "Wrong log/pass\n";
			}
			else
			{
				std::cout << "Auth succeed\n";
				while (1)
				{
					std::cout << std::endl;
					std::cout << "Enter command: ";
					std::string comm;
					std::cin.clear();
					std::getline(std::cin, comm);
					std::cout << std::endl;

					ClientExecutor exc(cn);
					std::wcout << exc.execute(STRINGtoWSTRING(comm));
					std::cin.clear();
				}
			}
		}
	}
	break;
	default:
		std::cout << "Wrong role\n";
		break;
	}
}
