#pragma once

#include "openssl/evp.h"

class EVPCipherContext {

	private:
		const EVP_CIPHER* m_cipher;

		const unsigned char* m_key;
		const unsigned char* m_iv;

	public:
		EVPCipherContext(const EVP_CIPHER*);
		~EVPCipherContext();

		const EVP_CIPHER* cipher() const {return m_cipher;}
		const unsigned char* key() const {return m_key;}
		const unsigned char* iv() const {return m_iv;}

	private:
		static unsigned char* rand(int size_in_bytes);
};