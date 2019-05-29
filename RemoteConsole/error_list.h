/*!
 * temporary function for errors
 */
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
	ERR_CONNECTION_LOST,
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
		std::wcerr << L"Error initialization library\n";
		break;
	case ERR_CREATE_SOCKET:
		std::wcerr << L"Error creation socket\n";
		break;
	case ERR_CREATE_CONNECTION:
		std::wcerr << L"Error create connection\n";
		break;
	case ERR_CONNECT_WITH_SERVER:
		std::wcerr << L"Error connect with server\n";
		break;
	case ERR_SENDING:
		std::wcerr << L"Error sending\n";
		break;
	case ERR_CHECKING_LOGIN:
		std::wcerr << L"Error checking login/password\n";
		break;
	case ERR_CONNECTION_LOST:
		std::wcerr << L"Connection have been lost\n";
		break;
	default:
		std::wcerr << L"Unknown error\n";
	}
}