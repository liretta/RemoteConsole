#pragma once
#include "icryptor.h"
#include "converting_functions.hpp"

class ClientCryptor: public ICryptor
{
	//byte generateKey(); //generate async or sync key
	//virtual void setKey(std::wstring) = 0;
	std::vector<char> encrypt(std::wstring message) override;
	std::wstring decrypt(std::vector<char> message) override;
};

std::vector<char> ClientCryptor::encrypt(std::wstring message)
{
	//TEST CASE!! CHANGE IT!!!
	std::string str = WSTRINGtoSTRING(message);

	std::vector<char> vc2;
	for (unsigned int i = 0; i < str.size(); ++i)
	{
		vc2.push_back(str[i]);
	};
	return std::move((vc2));
}

std::wstring ClientCryptor::decrypt(std::vector<char> message)
{
	std::string new_str;
	for (unsigned int i = 0; i < message.size(); ++i)
	{
		new_str.push_back(message[i]);
	}

	return std::move(STRINGtoWSTRING(new_str));
}
