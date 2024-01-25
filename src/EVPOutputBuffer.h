#pragma once

#include "openssl/evp.h"

class EVPOutputBuffer {

	private:
		unsigned char* m_buffer;
		int m_size;

	public:
		EVPOutputBuffer(const EVP_CIPHER*, int);
		~EVPOutputBuffer();

		unsigned char* buffer() const {return m_buffer;}
		int size() const {return m_size;}

		void allocate(int);

	private:
		void cleanup();
};