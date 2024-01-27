#include "ECSecretContext.h"
#include "Benchmark.h"

#include "openssl/obj_mac.h"

ECSecretContext::ECSecretContext()
	: m_yourPkey(nullptr)
	, m_peerPkey(nullptr)
	, m_secret(nullptr) {

	Benchmark benchmark;
	benchmark.save();

	m_yourPkey = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
	if (EC_KEY_generate_key(m_yourPkey) == 0) {
		cleanup();
		return;
	}

	m_peerPkey = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
	if (EC_KEY_generate_key(m_peerPkey) == 0) {
		cleanup();
		return;
	}
	
	const int field_size = EC_GROUP_get_degree(EC_KEY_get0_group(m_yourPkey));
	const int shared_secret_size = (field_size + 7) / 8;

	m_secret = new unsigned char[shared_secret_size];
	ECDH_compute_key(m_secret, shared_secret_size, EC_KEY_get0_public_key(m_peerPkey), m_yourPkey, NULL);

	benchmark.save();
	benchmark.print();
}

ECSecretContext::~ECSecretContext() {
	cleanup();
}

void ECSecretContext::cleanup() {

	if (m_yourPkey) {
		EC_KEY_free(m_yourPkey);
		m_yourPkey = nullptr;
	}

	if (m_peerPkey) {
		EC_KEY_free(m_peerPkey);
		m_peerPkey = nullptr;
	}

	if (m_secret) {
		delete[] m_secret;
		m_secret = 0;
	}
}