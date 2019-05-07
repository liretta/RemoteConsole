/*test networker
*
*/
#include <iostream>
#include "client_networker.h"
#include "server_networker.h"
#include "error_list.h"

int main(int argc, char** argv[])
{
	char choise = 'e';
	if (argc <= 1)
	{
		std::cout << "Please, choice you role: s(server) or c(client):";
		std::cin >> choise;
	}

	switch(choise)
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

				bool res = true;
				do
				{
					res = sn.send("test string");

				} while (!res);
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
				std::string tmpStr = "test";
				do
				{
					tmpStr = cn.receive();
					std::cout << tmpStr << std::endl;
				} while (tmpStr != "" || tmpStr!= "test");
			}
		}
			break;
		default:
		std::cout << "Wrong role\n";
		break;
	}

	system("pause");
	return 0;
}