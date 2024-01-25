// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cassert>

#include "EVPCipher.h"

namespace {
    static const unsigned char data[] = "Flying ad-hoc networks based on unmanned aerial vehicles are characterized by frequent changes in the network topology, which is caused by switching nodes on and off, as well as their movements in space. These aspects of the FANET operation determine the relevance of the development of new routing protocols, which differ significantly from the protocols of hierarchical networks with a constant topology. One of the key problems of FANET is the high overhead costs for transmitting service messages about the coordinates of network nodes, which is caused by the high mobility of these nodes. To reduce an amount of service information, FANET, we have developed an algorithm for constructing the UAV trajectory based on approximation by continuous piecewise linear functions. The developed algorithm allows for an approximation with an adaptive step. The use of this approach leads to increase in accuracy, since the approximating function adapts to the change in the nonlinearity properties of the initial characteristic.";
}

int main() {

    EVPCipher::test_evp_cipher(EVP_aes_128_cbc(), data, sizeof(data));
    EVPCipher::test_evp_cipher(EVP_aes_192_cbc(), data, sizeof(data));
    EVPCipher::test_evp_cipher(EVP_aes_256_cbc(), data, sizeof(data));

    return 0;
}