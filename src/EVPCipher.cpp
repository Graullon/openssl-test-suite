#include "EVPCipher.h"

#include <iostream>
#include "Benchmark.h"

bool EVPCipher::evp_cipher(
	const EVPCipherContext& cipher_context,
	EVPBuffer* output_buffer,
    const EVPBuffer& input_buffer,
	bool enc
) {
    const int cipher_block_size = EVP_CIPHER_block_size(cipher_context.cipher());
    const int minimum_required_output_space_in_bytes = input_buffer.size() + (enc ? cipher_block_size : -cipher_block_size);
    output_buffer->allocate(minimum_required_output_space_in_bytes);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if (EVP_CipherInit_ex2(ctx, cipher_context.cipher(), NULL, NULL, enc, NULL) == 0) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    std::cout << "Ciphering algorithm: " << EVP_CIPHER_name(cipher_context.cipher()) << std::endl;
    std::cout << "Operation: " << (enc ? "Encryption" : "Decryption") << std::endl;
    std::cout << "Key: " << cipher_context.key() << std::endl;
    std::cout << "IV: " << cipher_context.iv() << std::endl << std::endl;

    std::cout << "Input data (size in bytes: " << input_buffer.size() << "): \n" << input_buffer.data() << std::endl << std::endl;

    if (EVP_CipherInit_ex2(ctx, NULL, cipher_context.key(), cipher_context.iv(), enc, NULL) == 0) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    int output_data_length = 0;
    if (EVP_CipherUpdate(ctx, output_buffer->data(), &output_data_length, input_buffer.data(), input_buffer.size()) == 0) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    if (EVP_CipherFinal_ex(ctx, (output_buffer->data() + output_data_length), &output_data_length) == 0) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    std::cout << "Output data (size in bytes: " << output_buffer->size() << "): \n" << output_buffer->data() << std::endl;
    std::cout << std::endl;

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

void EVPCipher::test_evp_cipher(
    const EVPCipherContext& cipher_context,
    const EVPBuffer& input_buffer
) {

    Benchmark benchmark;
    benchmark.save();

    EVPBuffer encrypted_data(cipher_context.cipher());
    if (!evp_cipher(cipher_context, &encrypted_data, input_buffer, true)) {
        return;
    }

    benchmark.save();

    EVPBuffer unencrypted_data(cipher_context.cipher());
    if (!evp_cipher(cipher_context, &unencrypted_data, encrypted_data, false)) {
        return;
    }

    benchmark.save();
    benchmark.print();
}