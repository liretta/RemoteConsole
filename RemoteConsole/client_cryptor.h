/*! 
 * encryption functionality in client side:
 * - get public key and encrypt message with it
 * - generate synchronous key and initialize vector for it, encryption and decryption message via synchronous key
 * - pack public key for sending
 */
#pragma once
#include "i_cryptor.h"
#include "converting_functions.hpp"

class ClientCryptor: public ICryptor
{
private:
	//synchronous part:
	CryptoPP::byte m_sync_key[CryptoPP::Blowfish::DEFAULT_KEYLENGTH]; //synchronous key 
	CryptoPP::byte m_init_vector[CryptoPP::Blowfish::BLOCKSIZE]; //vector for initialize synchronous key
	CryptoPP::CFB_Mode<CryptoPP::Blowfish>::Encryption m_encryptor;
	CryptoPP::CFB_Mode<CryptoPP::Blowfish>::Decryption m_decryptor;

	//asynchronous part:
	CryptoPP::AutoSeededRandomPool m_rng;
	CryptoPP::RSA::PublicKey m_public_key;
	CryptoPP::RSAES_OAEP_SHA_Encryptor m_key_encryptor;
	CryptoPP::RSA::PublicKey repackPublicKey(const std::vector <char>& v_key);

public:
	ClientCryptor() = default;
	~ClientCryptor() = default;
	bool generateKey() override; //generate synchronous key and initialize vector for him
	bool setKey(const std::vector<char>& v_key); 
	std::vector<char> keyEncrypt(); //encryption synchronous key via public key
	std::vector<char> ivEncrypt(); //encryption synchronous initialize vector via public key
	std::vector<char> encrypt(const std::wstring& message) override; //synchronous encryption
	std::wstring decrypt(const std::vector<char>& message) override; //synchronous decryption
};

