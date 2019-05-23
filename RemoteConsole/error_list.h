//temporary function for errors
#pragma once
#include <iostream>

enum Error
{
	OK,
	ERR_LIBRARY_INIT,
	ERR_CREATE_SOCKET,
	ERR_CREATE_CONNECTION,
	ERR_CONNECT_WITH_SERVER,
	ERR_SENDING,
	ERR_CHECKING_LOGIN,
	ERR_UNKNOWN_ERROR
};

static void PrintError(Error er)
{
	switch (er)
	{
	case OK:
	std:: cout << "Operation successful\n";
	break;
	case ERR_LIBRARY_INIT:
		std::cout << "Error initialization library\n";
		break;
	case ERR_CREATE_SOCKET:
		std::cout << "Error creation socket\n";
		break;
	case ERR_CREATE_CONNECTION:
		std::cout << "Error create connection\n";
		break;
	case ERR_CONNECT_WITH_SERVER:
		std::cout << "Error connect with server\n";
		break;
	case ERR_SENDING:
		std::cout << "Error sending\n";
		break;
	case ERR_CHECKING_LOGIN:
		std::cout << "Error checking login/password\n";
	default:
		std::cout << "Unknown error\n";
	}
}