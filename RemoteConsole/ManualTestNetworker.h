/*!
 * temp test for checking networker
 * delete it after create unit-testing
 */
#pragma once
#include <iostream>
#include "error_list.h"
#include "server_networker.h"
#include "client_networker.h"


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
		}
		else
		{
			std::cout << "Connection created\n";

			bool res = true;
			do
			{
				res = sn.send("test string");

			} while (1);
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
		}
		else
		{
			std::cout << "Connection created\n";
			std::string tmpStr = "test";
			do
			{
				tmpStr = cn.receive();
				std::cout << tmpStr << std::endl;
			} while (tmpStr!="test string");
		}
	}
	break;
	default:
		std::cout << "Wrong role\n";
		break;
	}
}
