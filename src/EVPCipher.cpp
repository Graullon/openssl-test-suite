#include "EVPCipher.h"

#include <iostream>
#include "Benchmark.h"

bool EVPCipher::evp_cipher(
	const EVPCipherContext& cipher_context,
	EVPBuffer* output_buffer,
    const EVPBuffer& input_buffer,
	bool enc
) {

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if (EVP_CipherInit_ex(ctx, cipher_context.cipher(), NULL, NULL, NULL, enc) == 0) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    const int cipher_block_size = EVP_CIPHER_block_size(cipher_context.cipher());
    const int cipher_tag_length = EVP_CIPHER_CTX_tag_length(ctx);
    const int cipher_data_length = cipher_block_size + cipher_tag_length;

    const int minimum_required_output_space_in_bytes = input_buffer.size() + (enc? cipher_data_length : -cipher_data_length);
    output_buffer->allocate(minimum_required_output_space_in_bytes);

    #ifndef BENCHMARK_MODE
    std::cout 
        << "\nAlgorithm: " << EVP_CIPHER_name(cipher_context.cipher())
        << "\nOperation: " << (enc ? "Encryption" : "Decryption")
        << "\nKey: " << cipher_context.key()
        << "\nIV: " << cipher_context.iv()
        << "\nInput data (size in bytes: " << input_buffer.size() << ")" 
        << std::endl;
    #endif

    // Setup Tag
    if (!enc && (cipher_tag_length != 0)) {

        switch ( EVP_CIPHER_type(cipher_context.cipher()) ) {
            case NID_aes_128_gcm:
            case NID_aes_192_gcm:
            case NID_aes_256_gcm: {

                if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, cipher_tag_length, (input_buffer.data() + minimum_required_output_space_in_bytes)) == 0) {
                    EVP_CIPHER_CTX_free(ctx);
                    return false;
                }

            } break;
            case NID_aes_128_ccm:
            case NID_aes_192_ccm:
            case NID_aes_256_ccm: {

                if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_SET_TAG, cipher_tag_length, (input_buffer.data() + minimum_required_output_space_in_bytes)) == 0) {
                    EVP_CIPHER_CTX_free(ctx);
                    return false;
                }

            } break;
            default: return false;
        }
    }

    if (EVP_CipherInit_ex(ctx, NULL, NULL, cipher_context.key(), cipher_context.iv(), enc) == 0) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    int output_data_length = 0;
    int input_data_length = input_buffer.size();

    if (!enc && (cipher_tag_length != 0)) {
        input_data_length -= cipher_data_length;
    }

    int operation_data_length = 0;

    if (EVP_CipherUpdate(ctx, (output_buffer->data() + output_data_length), &operation_data_length, input_buffer.data(), input_data_length) == 0) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    output_data_length += operation_data_length;

    if (EVP_CipherFinal_ex(ctx, (output_buffer->data() + output_data_length), &operation_data_length) == 0) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    output_data_length += operation_data_length;

    if (enc && (cipher_tag_length != 0)) {

        switch (EVP_CIPHER_type(cipher_context.cipher())) {
            case NID_aes_128_gcm:
            case NID_aes_192_gcm:
            case NID_aes_256_gcm: {

                if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, cipher_tag_length, (output_buffer->data() + output_data_length)) == 0) {
                    EVP_CIPHER_CTX_free(ctx);
                    return false;
                }

            } break;
            case NID_aes_128_ccm:
            case NID_aes_192_ccm:
            case NID_aes_256_ccm: {

                if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_GET_TAG, cipher_tag_length, (output_buffer->data() + output_data_length)) == 0) {
                    EVP_CIPHER_CTX_free(ctx);
                    return false;
                }

            } break;
            default: return false;
        }
    }

    #ifndef BENCHMARK_MODE
    std::cout << "Output data (size in bytes: " << output_buffer->size() << ")" << std::endl;
    #endif

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

    #ifndef BENCHMARK_MODE
    const std::string_view input_buffer_string_view((const char*)input_buffer.data(), input_buffer.size());
    const std::string_view unencrypted_buffer_string_view((const char*)input_buffer.data(), input_buffer.size());

    std::cout
        << "\nUnecnrypted buffer equals Input buffer: " << (input_buffer_string_view.compare(unencrypted_buffer_string_view) == 0 ? "true" : "false")
        << std::endl;
    #endif

    benchmark.print();
}