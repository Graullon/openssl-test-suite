#pragma once

#include "openssl/ec.h"

class ECSecretContext {

	private:
		EC_KEY* m_yourPkey;
		EC_KEY* m_peerPkey;

		unsigned char* m_secret;

	public:
		ECSecretContext();
		~ECSecretContext();

		const EC_KEY* yourPkey() const {return m_yourPkey;}
		const EC_KEY* peerPkey() const {return m_peerPkey;}

		const unsigned char* secret() const {return m_secret;}

	private:
		void cleanup();
};