/*!
 * IN PROGRESS
 * Here will be all functionality for encrypting and decrypting messages in the server side
 * now there are stubs
 */
#pragma once
#include "i_cryptor.h"
#include "converting_functions.hpp"

class ServerCryptor : public ICryptor
{
	//byte generateKey(); //generate async or sync key
	//virtual void setKey(std::wstring) = 0;
	//std::vector<char> encrypt(std::wstring message) override;
	//std::wstring decrypt(std::vector<char> message) override;
public:
	std::vector<char> encrypt(std::wstring message);
	std::wstring decrypt(std::vector<char> message);
};
