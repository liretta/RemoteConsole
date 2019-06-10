#include "client_cryptor.h"

/*!
 * get vector <char> - key from server
 * convert it into string and load from this string the public key
 * @return key
 */
CryptoPP::RSA::PublicKey ClientCryptor::repackPublicKey(const std::vector<char>& v_key)
{
	CryptoPP::RSA::PublicKey tmp_key;
	std::string str_for_key(v_key.begin(), v_key.end());
	
	try
	{
		tmp_key.Load(CryptoPP::StringStore((const byte*)str_for_key.data(), str_for_key.size()).Ref());
	}
	catch (const CryptoPP::BERDecodeErr& ex)
	{
		std::wcerr << ex.what() << std::endl;
	}
	return tmp_key;
}

/*!
 * generate synchronous key and initialize_vector
 * @return true if key and init-vector was generated 
 */
bool ClientCryptor::generateKey()
{
	m_rng.GenerateBlock(m_sync_key, sizeof(m_sync_key));
	m_rng.GenerateBlock(m_init_vector, sizeof(m_init_vector));
	m_encryptor.SetKeyWithIV(m_sync_key, sizeof(m_sync_key), m_init_vector, sizeof(m_init_vector));
	m_decryptor.SetKeyWithIV(m_sync_key, sizeof(m_sync_key), m_init_vector, sizeof(m_init_vector));
	return true;
}

/*!
 * get a public key, validate it and save it to m_publicKey
 */
bool ClientCryptor::setKey(const std::vector<char>& v_key)
{
	m_public_key = repackPublicKey(v_key);
	if (!m_public_key.Validate(m_rng, 3))
	{
		return false;
	}
	CryptoPP::RSAES_OAEP_SHA_Encryptor KeyEncryptor(m_public_key);
	m_key_encryptor = KeyEncryptor;
	return true;
}

/*!
 * convert synchronous key to string
 * encryption synchronous key via server public key
 * convert result string into vector <char>
 * @return result encryption message
 */
std::vector<char> ClientCryptor::keyEncrypt()
{
	int size = CryptoPP::Blowfish::DEFAULT_KEYLENGTH;
	std::vector<char> vc_key(size);

	//use bitmap copy because cryptoPP adds its bits to the key
	//if we use std::copy we will have incorrect data in m_synchronous key in general result
	for(int i = 0; i< size; ++i) 
	{
		vc_key[i] = m_sync_key[i];
	}
	std::string tmp_key(vc_key.begin(), vc_key.end());
	std::string result_key;

	CryptoPP::StringSource(tmp_key, true, new CryptoPP::PK_EncryptorFilter(m_rng, m_key_encryptor, new CryptoPP::StringSink(result_key)));
	vc_key.clear();
	std::copy(result_key.begin(), result_key.end(), std::back_inserter(vc_key));
	return vc_key;
}

/*!
 * convert initialize vector for synchronous encryption to string
 * encryption synchronous key via server public key
 * convert result string into vector <char>
 * @return result encryption message
 */
std::vector<char> ClientCryptor::ivEncrypt()
{
	int size = CryptoPP::Blowfish::BLOCKSIZE;
	std::vector<char> vc_iv(size);

	//use bitmap copy because cryptoPP adds its bits to the key
	//if we use std::copy we will have incorrect data in m_synchronous key in general result
	for(int i = 0; i< size; ++i)
	{
		vc_iv[i] = m_init_vector[i];
	}
	std::string tmp_iv(vc_iv.begin(), vc_iv.end());
	std::string result_iv;

	CryptoPP::StringSource(tmp_iv, true, new CryptoPP::PK_EncryptorFilter(m_rng, m_key_encryptor, new CryptoPP::StringSink(result_iv)));
	vc_iv.clear();
	std::copy(result_iv.begin(), result_iv.end(), std::back_inserter(vc_iv));
	return vc_iv;
}

/*!
 * get wstring message to synchronous encryption
 * convert wstring message to sting, encrypted and convert encrypted string to vector <char>
 * @return encrypted message in format vector <char>
 */
std::vector<char> ClientCryptor::encrypt(const std::wstring& message)
{
	std::string result_str, message_str = (WSTRINGtoSTRING(message));
	std::vector<char> vc;
	if (m_sync_key != nullptr)
	{
		CryptoPP::StringSource(message_str, true, new CryptoPP::StreamTransformationFilter(m_encryptor, new CryptoPP::StringSink(result_str)));
		std::copy(result_str.begin(), result_str.end(), std::back_inserter(vc));
	}

	return vc;
}

/*!
 * get message that was encryption via synchronous encryption
 * convert message to string, decryption it and convert result to wstring
 * @return decrypted message in format wstring
 */
std::wstring ClientCryptor::decrypt(const std::vector<char>& message)
{
	std::string decrypt_str, encrypt_str(message.begin(), message.end());

	CryptoPP::StringSource(encrypt_str, true, new CryptoPP::StreamTransformationFilter(m_decryptor, new CryptoPP::StringSink(decrypt_str)));
	return (STRINGtoWSTRING(decrypt_str));
}