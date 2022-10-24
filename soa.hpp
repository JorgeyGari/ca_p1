//
// Created by laura on 23/10/2022.
//

#ifndef PRUEBA_CA_SOA_HPP
#define PRUEBA_CA_SOA_HPP

#include <vector>
#include <filesystem>

struct Image    // SOA we use to represent an image
{
    std::vector<uint8_t> r; // Vector containing the red values of every pixel in the image in 8 bits each
    std::vector<uint8_t> g; // Vector containing the green values of every pixel in the image in 8 bits each
    std::vector<uint8_t> b; // Vector containing the blue values of every pixel in the image in 8 bits each
};

#endif //PRUEBA_CA_SOA_HPP
