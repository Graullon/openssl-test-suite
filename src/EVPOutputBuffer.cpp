#include "EVPOutputBuffer.h"

#include <iostream>

EVPOutputBuffer::EVPOutputBuffer(const EVP_CIPHER* cipher, int inputBufferSize)
	: m_buffer(nullptr)
	, m_size(0) {}

EVPOutputBuffer::~EVPOutputBuffer() {
	cleanup();
}

void EVPOutputBuffer::allocate(int size) {

	cleanup();

	m_size = size;
	m_buffer = new unsigned char[m_size];

	memset(m_buffer, 0, m_size);
}

void EVPOutputBuffer::cleanup() {
	if (m_buffer) {
		delete[] m_buffer;
		m_buffer = nullptr;
	}
}