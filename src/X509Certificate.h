#pragma once

#include "openssl/evp.h"
#include "openssl/x509.h"

class EVPBuffer;

class X509Certificate {

	public:
		enum class KeyType { RSA = 0, EC };

	private:
		EVP_PKEY* m_pkey;
		X509* m_certificate;

	public:
		X509Certificate(KeyType);
		~X509Certificate();

		EVP_PKEY* pkey() const {return m_pkey;}
		const X509* certificate() const {return m_certificate;}

		void sign();
		void sign(const X509Certificate&);

		bool verify(const X509Certificate&);
		EVPBuffer* derive(const X509Certificate&);

		void print();

		static void test_x509_certificate();

	private:
		void cleanup();
		void createCertificate();
};