#include "server_cryptor.h"
/*
 * save public key into string and convert it to vector<char>
 */
std::vector<char> ServerCryptor::getPublicKey()
{
	std::string str_for_save_key;
	std::vector<char> key;
	CryptoPP::StringSink save_source(str_for_save_key);
	m_public_key.Save(save_source);

	std::copy(str_for_save_key.begin(), str_for_save_key.end(), std::back_inserter(key));
	return key;
}

/*!
 * generate pair async keys - private & public
 * @return true if generate and validate new keys was successful
 */
bool ServerCryptor::generateKey()
{
	CryptoPP::InvertibleRSAFunction param;
	param.GenerateRandomWithKeySize(m_rng, KEY_SIZE);
	CryptoPP::RSA::PrivateKey tmp_pk(param);
	CryptoPP::RSA::PublicKey tmp_pubk(param);

	if (!tmp_pk.Validate(m_rng, 3) || !tmp_pubk.Validate(m_rng, 3))
	{
		std::wcerr << "Error validation keys" << std::endl;
		return false;
	}

	CryptoPP::RSAES_OAEP_SHA_Encryptor tmp_encr(tmp_pubk);
	CryptoPP::RSAES_OAEP_SHA_Decryptor tmp_decr(tmp_pk);

	m_private_key = tmp_pk;
	m_public_key = tmp_pubk;
	m_key_encryptor = tmp_encr;
	m_key_decryptor = tmp_decr;
	return true;
}

/*! 
 * set synchronous key and initialize vector 
 * decrypt it and initialize synchronous encryptor and decryptor
 * @return false 
 */
bool ServerCryptor::setKey(const std::vector<char>& key, const std::vector<char>& iv)
{
	if(!key_decrypt(key))
	{
		return false;
	}
	if(!iv_decrypt(iv))
	{
		return false;
	}
	
	m_encryptor.SetKeyWithIV(m_sync_key, sizeof(m_sync_key), m_init_vector, sizeof(m_init_vector));
	m_decryptor.SetKeyWithIV(m_sync_key, sizeof(m_sync_key), m_init_vector, sizeof(m_init_vector));
	return true;
}

/*!
 * get encryption synchronous key 
 * decrypt key via asynchronous encryption
 * @return true, if encryption was successful and key was saved in class member sync_key
 */
bool ServerCryptor::key_decrypt(const std::vector<char>& key)
{
	std::string tmp_encrypt_key, tmp_decrypt_key;
	tmp_encrypt_key.append(key.begin(), key.end());
	
	CryptoPP::StringSource(tmp_encrypt_key, true, new CryptoPP::PK_DecryptorFilter(m_rng, m_key_decryptor, new CryptoPP::StringSink(tmp_decrypt_key)));

	std::vector<char> tmp_key;
	std::copy(tmp_decrypt_key.begin(), tmp_decrypt_key.end(), std::back_inserter(tmp_key));
	for(int i = 0; i < tmp_key.size(); ++i)
	{
		m_sync_key[i] = tmp_key[i];
	}
	//std::copy(tmp_key.begin(), tmp_key.end(), m_sync_key);
	return true;
}

/*!
 * get encryption initialize vector for synchronous encryption
 * decrypt vector via asynchronous encryption
 * @return true, if encryption was successful and key was saved in class member init_vector
 */
bool ServerCryptor::iv_decrypt(const std::vector<char>& iv)
{
	std::string tmp_encrypt_iv, tmp_decrypt_iv;
	tmp_encrypt_iv.append(iv.begin(), iv.end());

	CryptoPP::StringSource(tmp_encrypt_iv, true, new CryptoPP::PK_DecryptorFilter(m_rng, m_key_decryptor, new CryptoPP::StringSink(tmp_decrypt_iv)));

	std::vector<char> tmp_iv;
	std::copy(tmp_decrypt_iv.begin(), tmp_decrypt_iv.end(), std::back_inserter(tmp_iv));
	std::copy(tmp_iv.begin(), tmp_iv.end(), m_init_vector);
	return true;
}

/*!
 * get wstring message to synchronous encryption
 * convert wstring message to sting, encrypted and convert encrypted string to vector <char>
 * @return encrypted message in format vector <char>
 */
std::vector<char> ServerCryptor::encrypt(const std::wstring& message)
{
	std::string result_str, message_str = (WSTRINGtoSTRING(message));

	CryptoPP::StringSource(message_str, true, new CryptoPP::StreamTransformationFilter(m_encryptor, new CryptoPP::StringSink(result_str)));

	std::vector<char> vc;
	std::copy(result_str.begin(), result_str.end(), std::back_inserter(vc));
	return vc;
}

/*!
 * get message that was encryption via synchronous encryption
 * convert message to string, decryption it and convert result to wstring
 * @return decrypted message in format wstring
 */
std::wstring ServerCryptor::decrypt(const std::vector<char>& message)
{
	std::string decrypt_str, encrypt_str(message.begin(), message.end());

	CryptoPP::StringSource(encrypt_str, true, new CryptoPP::StreamTransformationFilter(m_decryptor, new CryptoPP::StringSink(decrypt_str)));
	return (STRINGtoWSTRING(decrypt_str));
}