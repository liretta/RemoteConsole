/*!
 * common function for sending error message into wcerr
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
		std::cout << "Operation successful" << std::endl;
	break;
	case ERR_LIBRARY_INIT:
		std::wcerr << L"Error initialization library" << std::endl;
		break;
	case ERR_CREATE_SOCKET:
		std::wcerr << L"Error creation socket" << std::endl;
		break;
	case ERR_CREATE_CONNECTION:
		std::wcerr << L"Error create connection" << std::endl;
		break;
	case ERR_CONNECT_WITH_SERVER:
		std::wcerr << L"Error connect with server" << std::endl;
		break;
	case ERR_SENDING:
		std::wcerr << L"Error sending" << std::endl;
		break;
	case ERR_CHECKING_LOGIN:
		std::wcerr << L"Error checking login/password" << std::endl;
		break;
	case ERR_CONNECTION_LOST:
		std::wcerr << L"Connection have been lost" << std::endl;
		break;
	default:
		std::wcerr << L"Unknown error" << std::endl;
	}
}