#include "client_cryptor.h"

std::string ClientCryptor::encrypt(std::wstring message)
{
	//TEST CASE!! CHANGE IT!!!
	return (WSTRINGtoSTRING(message));


}

std::wstring ClientCryptor::decrypt(std::string message)
{
	return (STRINGtoWSTRING(message));
}
