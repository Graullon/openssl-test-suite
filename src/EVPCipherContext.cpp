#include "EVPCipherContext.h"

#include "openssl/rand.h"

EVPCipherContext::EVPCipherContext(const EVP_CIPHER* cipher)
	: m_cipher(cipher) 
	, m_key( rand(EVP_CIPHER_key_length(cipher)) )
	, m_iv( rand(EVP_CIPHER_iv_length(cipher)) ) {}

EVPCipherContext::~EVPCipherContext() {
	if (m_key) {
		delete[] m_key;
		m_key = nullptr;
	}
	if (m_iv) {
		delete[] m_iv;
		m_iv = nullptr;
	}
}

unsigned char* EVPCipherContext::rand(int size_in_bytes) {

	unsigned char* buf = new unsigned char[size_in_bytes];

	if (RAND_bytes(buf, size_in_bytes) != 1) {
		delete[] buf;
		return nullptr;
	}

	return buf;
}