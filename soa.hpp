#ifndef CA_P1_SOA_HPP
#define CA_P1_SOA_HPP

#include <filesystem>
#include <vector>

struct Image// SOA we use to represent an image
{
    std::vector<uint8_t> r;
    std::vector<uint8_t> g;
    std::vector<uint8_t> b;
};

#endif//CA_P1_SOA_HPP
