#pragma once

#include "EVPCipherContext.h"
#include "EVPBuffer.h"

class EVPCipher {		

	public:
		static bool evp_cipher(
			const EVPCipherContext& cipher_context,
			EVPBuffer* output_buffer,
			const EVPBuffer& input_buffer,
			bool enc
		);

		static void test_evp_cipher(
			const EVPCipherContext& cipher_context, 
			const EVPBuffer& input_buffer
		);

	private:
		EVPCipher() = delete;
};