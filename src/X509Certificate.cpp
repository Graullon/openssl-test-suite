﻿#include "X509Certificate.h"

#include "EVPBuffer.h"
#include "Benchmark.h"

#include <iostream>
#include <string_view>

#include "openssl/rsa.h"
#include "openssl/ec.h"
#include "openssl/bio.h"
#include "openssl/pem.h"

namespace {
	static long serial_number_counter = 1;
}

X509Certificate::X509Certificate(KeyType pkeyType)
	: m_pkey(nullptr)
	, m_certificate(nullptr) {

	switch (pkeyType) {
		case KeyType::RSA: {
			m_pkey = EVP_RSA_gen(2048);
		} break;
		case KeyType::EC: {
			m_pkey = EVP_EC_gen("secp521r1");
		} break;
		default: break;
	}
}

void X509Certificate::sign() {

	if (!m_pkey || m_certificate) {
		return;
	}

	createCertificate();

	X509_set_pubkey(m_certificate, m_pkey);

	X509_NAME* name = X509_get_subject_name(m_certificate);

	X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char*)"UA", -1, -1, 0);
	X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char*)"NAU UAVs", -1, -1, 0);
	X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)"localhost", -1, -1, 0);

	X509_set_issuer_name(m_certificate, name);
	X509_sign(m_certificate, m_pkey, EVP_sha256());

	#ifndef BENCHMARK_MODE
	print();
	#endif
}

void X509Certificate::sign(const X509Certificate& certificate) {

	if (!m_pkey || m_certificate) {
		return;
	}

	createCertificate();

	X509_set_issuer_name(m_certificate, X509_get_subject_name(certificate.certificate()));
	X509_set_subject_name(m_certificate, X509_get_subject_name(certificate.certificate()));

	X509_set_pubkey(m_certificate, certificate.pkey());
	X509_sign(m_certificate, certificate.pkey(), EVP_sha256());

	#ifndef BENCHMARK_MODE
	print();
	#endif
}

bool X509Certificate::verify(const X509Certificate& certificate) {
	return X509_verify(m_certificate, certificate.pkey()) == 1;
}

EVPBuffer* X509Certificate::derive(const X509Certificate& certificate) {

	EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(m_pkey, NULL);
	if (ctx == NULL) {
		return nullptr;
	}

	if (EVP_PKEY_derive_init(ctx) != 1) {
		return nullptr;
	}

	if (EVP_PKEY_derive_set_peer(ctx, certificate.pkey()) != 1) {
		return nullptr;
	}

	size_t secret_key_size = 0;
	if (EVP_PKEY_derive(ctx, NULL, &secret_key_size) != 1) {
		return nullptr;
	}

	unsigned char* secret_key_buffer = new unsigned char[secret_key_size];
	memset(secret_key_buffer, 0, secret_key_size);

	if (EVP_PKEY_derive(ctx, secret_key_buffer, &secret_key_size) != 1) {
		return nullptr;
	}

	EVPBuffer* secret_key = new EVPBuffer(secret_key_buffer, secret_key_size);
	delete[] secret_key_buffer;

	return secret_key;
}

void X509Certificate::print() {

	if (!m_certificate) {
		return;
	}
	
	BIO* bio = BIO_new(BIO_s_mem());

	X509_print(bio, m_certificate);
	//PEM_write_bio_X509(bio, m_certificate);
	//PEM_write_bio_PrivateKey(bio, m_pkey, NULL, NULL, 0, NULL, NULL);

	BUF_MEM* bio_internal_buffer = nullptr;
	BIO_get_mem_ptr(bio, &bio_internal_buffer);

	std::cout << std::endl << std::string_view(bio_internal_buffer->data, bio_internal_buffer->length) << std::endl << std::endl;

	BIO_free(bio);
}

X509Certificate::~X509Certificate() {
	cleanup();
}

void X509Certificate::cleanup() {
	if (m_certificate) {
		X509_free(m_certificate);
		m_certificate = nullptr;
	}
	if (m_pkey) {
		EVP_PKEY_free(m_pkey);
		m_pkey = nullptr;
	}
}

void X509Certificate::createCertificate() {

	m_certificate = X509_new();

	ASN1_INTEGER_set(X509_get_serialNumber(m_certificate), serial_number_counter++);

	X509_gmtime_adj(X509_getm_notBefore(m_certificate), 0);
	X509_gmtime_adj(X509_getm_notBefore(m_certificate), 31536000L); // valid for 1 year starting from now
}

void X509Certificate::test_x509_certificate() {

	Benchmark benchmark;
	benchmark.save();

	X509Certificate caSignedCertificate(X509Certificate::KeyType::EC);
	caSignedCertificate.sign();

	benchmark.save();

	X509Certificate csrCertificate(X509Certificate::KeyType::EC);
	csrCertificate.sign(caSignedCertificate);

	benchmark.save();

	#ifndef BENCHMARK_MODE
	std::cout 
		<< "\nCSR Certificate signed by CA Certificate: " << ((csrCertificate.verify(caSignedCertificate) == true) ? "true" : "false") 
		<< std::endl;
	#endif

	benchmark.print();

	EVPBuffer* caCertificateSecretKey = caSignedCertificate.derive(csrCertificate);
	EVPBuffer* csrCertificateSecretKey = csrCertificate.derive(caSignedCertificate);

	#ifndef BENCHMARK_MODE
	const std::string_view ca_certificate_secret_key_string_view((const char*)caCertificateSecretKey->data(), caCertificateSecretKey->size());
	const std::string_view csr_certificate_secret_key_string_view((const char*)csrCertificateSecretKey->data(), csrCertificateSecretKey->size());

	std::cout
		<< "\nCA&CSR certificate secret key equals CSR&CA certificate secret key: " << (ca_certificate_secret_key_string_view.compare(csr_certificate_secret_key_string_view) == 0 ? "true" : "false")
		<< std::endl;
	#endif

	delete caCertificateSecretKey;
	delete csrCertificateSecretKey;

	// Uncomment if want to check third-party certificate sign
	#ifndef BENCHMARK_MODE
	X509Certificate thirdPartyCertificate(X509Certificate::KeyType::EC);
	thirdPartyCertificate.sign();

	std::cout
		<< "\nCSR Certificate signed by CA Certificate: " << ((csrCertificate.verify(caSignedCertificate) == true) ? "true" : "false")
		<< std::endl;

	EVPBuffer* thirdPartyCertificateSecretKey = caSignedCertificate.derive(thirdPartyCertificate);

	const std::string_view third_party_certificate_secret_key_string_view((const char*)thirdPartyCertificateSecretKey->data(), thirdPartyCertificateSecretKey->size());

	std::cout
		<< "\nCA&ThirdParty certificate secret key equals CA&CSR certificate secret key: " << (ca_certificate_secret_key_string_view.compare(third_party_certificate_secret_key_string_view) == 0 ? "true" : "false")
		<< std::endl;
	
	delete thirdPartyCertificateSecretKey;
	#endif
}