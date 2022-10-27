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

std::vector<struct Pixel> read_pixels(const std::filesystem::path &path, uint32_t start, uint32_t width, uint32_t height);

void write_bmp(std::filesystem::path &path, const Header &header, std::vector<Pixel> image);

std::vector<Pixel> perform_op(std::vector<Pixel> image, std::string &op, const std::filesystem::path& new_file, const Header &header);

#endif//CA_P1_AOS_HPP
