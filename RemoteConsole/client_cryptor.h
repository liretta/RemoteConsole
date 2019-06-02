/*! 
 * IN PROGRESS
 * Here will be all functionality for encrypting and decrypting messages in the client side
 * now there are stubs
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

	//acync part:
	CryptoPP::AutoSeededRandomPool m_rng;
	CryptoPP::RSA::PublicKey m_public_key;
	CryptoPP::RSAES_OAEP_SHA_Encryptor m_key_encryptor;
	CryptoPP::RSA::PublicKey repackPublicKey(std::vector <char> v_key);

public:
	ClientCryptor() = default;
	~ClientCryptor() = default;
	bool generateKey() override;
	bool setKey(std::vector<char> v_key);
	std::vector<char> keyEncrypt();
	std::vector<char> ivEncrypt();
	//std::vector<char> publicEncrypt(std::wstring message);
	std::vector<char> encrypt(std::wstring message) override;
	std::wstring decrypt(std::vector<char> message) override;
};

