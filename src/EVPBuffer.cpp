#include "EVPBuffer.h"

#include <iostream>

EVPBuffer::EVPBuffer(const unsigned char* data, int size)
	: m_data(new unsigned char[size])
	, m_size(size) {

	memcpy(m_data, data, m_size);
}

EVPBuffer::EVPBuffer(const EVP_CIPHER* cipher)
	: m_data(nullptr)
	, m_size(0) {}

EVPBuffer::~EVPBuffer() {
	cleanup();
}

void EVPBuffer::allocate(int size) {

	cleanup();

	m_size = size;
	m_data = new unsigned char[m_size];

	memset(m_data, 0, m_size);
}

void EVPBuffer::cleanup() {
	if (m_data) {
		delete[] m_data;
		m_data = nullptr;
	}
}