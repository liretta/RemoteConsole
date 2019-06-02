/*!
 * temporary manual test for checking server-client class
 * will be deleted after creating unit-tests
 */
#pragma once
#include <iostream>
#include <locale>
#include "error_list.h"
#include "server_networker.h"
#include "client_networker.h"
#include "client_logger.h"
#include "server_logger.h"
#include "client_executor.h"
#include "server_executor.h"
#include "class_server.h"
#include "class_client.h"

void TestClassServer()
{
	std::wcout.imbue(std::locale("rus_rus.866"));
	std::wcin.imbue(std::locale("rus_rus.866"));

	char choice = 'e';
	std::cout << "Please, choice you role: s(server) or c(client):";
	std::cin >> choice;

	switch (choice)
	{
	case 's':
		{
		Server sv;
		sv.waitingForConnection();
		while (true)
		{
			sv.sendKey();
			sv.getKey();
			sv.logIn();
			sv.run();
			sv.reconnect();
		}
		}
		break;
	case 'c':
		{
		Client cl;
			ClientNetworker &cn = cl.getNetworker();
			Error er = cn.init("127.0.0.1");
			if (er != OK)
			{
				PrintError(er);
				break;
			}
			else
			{
				std::cout << "Connection created\n";
				std::cout << "Waiting for key...\n";
				cl.setCryptoKey();
				cl.sendCryptoKey();
				std::string log, pas;
				std::cout << "Login: ";
				std::cin >> log;
				std::cout << "Pass: ";
				std::cin >> pas;

				ClientLogger &logger = cl.getLogger();
				if (!logger.check_password(std::make_pair(log, pas)))
				{
					std::cout << "Wrong log/pass\n";
				}
				else
				{
					std::cout << "Auth succeed\n";
					while (1)
					{

						std::cin.clear();
						std::cin.ignore();
						std::cout << std::endl;
						std::cout << "Enter command: ";
						std::string comm;
						std::getline(std::cin, comm);
						std::cout << std::endl;
						//std::cin.clear();
						if (comm == "exit")
							break;
						ClientExecutor& exc = cl.getExecutor();
						std::wstring to_show = exc.execute(STRINGtoWSTRING(comm));
						std::wcout << to_show << std::endl;
					}
				}
			}
		}
	}
}