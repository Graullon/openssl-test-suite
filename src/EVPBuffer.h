#pragma once

#include "openssl/evp.h"

class EVPBuffer {

	private:
		unsigned char* m_data;
		int m_size;

	public:
		EVPBuffer(const unsigned char*, int);
		EVPBuffer(const EVP_CIPHER*);
		~EVPBuffer();

		unsigned char* data() const {return m_data;}
		int size() const {return m_size;}

		void allocate(int);

	private:
		void cleanup();
};