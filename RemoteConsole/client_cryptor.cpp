#include "client_cryptor.h"

std::vector<char> ClientCryptor::encrypt(std::wstring message)
{
	//TEST CASE!! IT WILL BE CHANGED!!!
	std::string temp_str = (WSTRINGtoSTRING(message));
	std::vector<char> vc;
	std::copy(temp_str.begin(), temp_str.end(), std::back_inserter(vc));
	return vc;
}

std::wstring ClientCryptor::decrypt(std::vector<char> message)
{
	//TEST CASE!! IT WILL BE CHANGED!!!
	std::string temp_str (message.begin(), message.end());
	return (STRINGtoWSTRING(temp_str));
}