#pragma once
//interface class
#include "../cryptopp820/osrng.h" //for generate key and init. vector
#include "../cryptopp820/cryptlib.h" //for exceptions
#include "../cryptopp820/hex.h" //for hex viewing
#include "../cryptopp820/filters.h" //converter input into cipher
#include "../cryptopp820/aes.h"
#include "../cryptopp820/modes.h"
#include "../cryptopp820/md5.h"
#include "../cryptopp820/blowfish.h"
#include "../cryptopp820/rsa.h"

#include <string>
#include <vector>
#include <iostream>

class ICryptor
{
public:
	virtual bool generateKey() = 0;
	virtual std::vector<char> encrypt (std::wstring message) = 0;
	virtual std::wstring decrypt(std::vector<char> message) = 0;

};

