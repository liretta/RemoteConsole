/*!
 * IN PROGRESS
 * Here will be all functionality for encrypting and decrypting messages in the server side
 * now there are stubs
 */
#pragma once
#include "i_cryptor.h"
#include "converting_functions.hpp"

const int KEY_SIZE = 3072; //size recommended by CryptoPP

class ServerCryptor : public ICryptor
{
private:
	//synchronous part:
	CryptoPP::byte m_sync_key[CryptoPP::Blowfish::DEFAULT_KEYLENGTH]; //synchronous key 
	CryptoPP::byte m_init_vector[CryptoPP::Blowfish::BLOCKSIZE]; //vector for initialize synchronous key
	CryptoPP::CFB_Mode<CryptoPP::Blowfish>::Encryption m_encryptor;
	CryptoPP::CFB_Mode<CryptoPP::Blowfish>::Decryption m_decryptor;

	//acync part:
	CryptoPP::AutoSeededRandomPool m_rng; 
	CryptoPP::RSA::PrivateKey m_private_key;
	CryptoPP::RSA::PublicKey m_public_key;
	CryptoPP::RSAES_OAEP_SHA_Encryptor m_key_encryptor; 
	CryptoPP::RSAES_OAEP_SHA_Decryptor m_key_decryptor;
	bool key_decrypt(std::vector<char> key);
	bool iv_decrypt(std::vector<char> iv);

public:
	ServerCryptor() = default;
	~ServerCryptor() = default;
	bool generateKey() override;
	std::vector<char> getPublicKey();
	bool setKey(std::vector<char> key, std::vector<char> iv);

	
	std::vector<char> encrypt(std::wstring message) override;
	std::wstring decrypt(std::vector<char> message) override;
};
