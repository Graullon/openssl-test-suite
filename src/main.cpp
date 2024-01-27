// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>

#include "EVPCipher.h"
#include "EVPBuffer.h"

#include "X509Certificate.h"

namespace {
    static const unsigned char data[] = "Flying ad-hoc networks based on unmanned aerial vehicles are characterized by frequent changes in the network topology, which is caused by switching nodes on and off, as well as their movements in space. These aspects of the FANET operation determine the relevance of the development of new routing protocols, which differ significantly from the protocols of hierarchical networks with a constant topology. One of the key problems of FANET is the high overhead costs for transmitting service messages about the coordinates of network nodes, which is caused by the high mobility of these nodes. To reduce an amount of service information, FANET, we have developed an algorithm for constructing the UAV trajectory based on approximation by continuous piecewise linear functions. The developed algorithm allows for an approximation with an adaptive step. The use of this approach leads to increase in accuracy, since the approximating function adapts to the change in the nonlinearity properties of the initial characteristic.";
}

int main() {
    
    #ifndef BENCHMARK_MODE
    std::cout 
        << "---------------------------------------------------------------------\n"
        << "Input data:\n" << data 
        << std::endl;
    #endif

    const EVPBuffer input_buffer(data, sizeof(data));

    EVPCipher::test_evp_cipher(EVP_aes_128_gcm(), input_buffer);
    EVPCipher::test_evp_cipher(EVP_aes_192_gcm(), input_buffer);
    EVPCipher::test_evp_cipher(EVP_aes_256_gcm(), input_buffer);
    
    X509Certificate::test_x509_certificate();

    return 0;
}