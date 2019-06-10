/*!
 * functionality for encrypting and decrypting messages in the server side
 * - generate and validate public&private keys
 * - set synchronous key
 * - can encrypt/decrypt message with sychronous key 
 * - decrypt message via private key
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

	//async part:
	CryptoPP::AutoSeededRandomPool m_rng; 
	CryptoPP::RSA::PrivateKey m_private_key;
	CryptoPP::RSA::PublicKey m_public_key;
	CryptoPP::RSAES_OAEP_SHA_Encryptor m_key_encryptor; 
	CryptoPP::RSAES_OAEP_SHA_Decryptor m_key_decryptor;
	bool key_decrypt(const std::vector<char>& key);
	bool iv_decrypt(const std::vector<char>& iv);

public:
	ServerCryptor() = default;
	~ServerCryptor() = default;
	bool generateKey() override; //generate and validate pair private/public keys
	std::vector<char> getPublicKey(); //return public key for sending as vector <char>
	bool setKey(const std::vector<char>& key, const std::vector<char>& iv); //set synchronous key and decrypt their via private key

	std::vector<char> encrypt(const std::wstring& message) override; //synchronous encryption
	std::wstring decrypt(const std::vector<char>& message) override; //synchronous decryption
};
