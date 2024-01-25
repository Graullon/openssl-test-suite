#pragma once

#include "EVPCipherContext.h"
#include "EVPOutputBuffer.h"

class EVPCipher {		

	public:
		static bool evp_cipher(
			const EVPCipherContext& cipher_context,
			EVPOutputBuffer* output_buffer,
			const unsigned char* input_data,
			int input_data_length,
			bool enc
		);

		static void test_evp_cipher(
			const EVPCipherContext& cipher_context, 
			const unsigned char* data,
			int data_size
		);

	private:
		EVPCipher() = delete;
};