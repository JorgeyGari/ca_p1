#ifndef CA_P1_AOS_HPP
#define CA_P1_AOS_HPP

#include "common_rw.hpp"
#include <filesystem>
#include <vector>

struct Pixel// Structure that the AOS will hold, represents one pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

void perform_op(const std::vector<Pixel> &image, std::string &op, std::filesystem::path new_file, const Header &header);

#endif//CA_P1_AOS_HPP
