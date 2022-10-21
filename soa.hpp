#ifndef CA_P1_SOA_HPP
#define CA_P1_SOA_HPP

#include <vector>
#include <filesystem>

struct Image    // SOA we use to represent an image
{
    std::vector<uint8_t> r; // Vector containing the red values of every pixel in the image in 8 bits each
    std::vector<uint8_t> g; // Vector containing the green values of every pixel in the image in 8 bits each
    std::vector<uint8_t> b; // Vector containing the blue values of every pixel in the image in 8 bits each
};

#endif //CA_P1_SOA_HPP
