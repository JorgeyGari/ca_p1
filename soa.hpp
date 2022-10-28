/* Header containing definitions exclusive to the SOA version */

#ifndef CA_P1_SOA_HPP
#define CA_P1_SOA_HPP

#include "common_rw.hpp"
#include "common_gauss.hpp"
#include "common_hst.hpp"
#include "common_mono.hpp"
#include <filesystem>
#include <vector>

struct Image
// SOA we use to represent an image
{
    std::vector<uint8_t> r;
    std::vector<uint8_t> g;
    std::vector<uint8_t> b;
};

Image read_pixels(const std::filesystem::path &path, uint32_t start, uint32_t width, uint32_t height);

void write_bmp(std::filesystem::path &path, const Header &header, Image image);

void frequencies (const std::vector<uint8_t> &color, std::ofstream& f);

std::vector<int> getmim (int i, int j, const Header &h, const std::vector<uint8_t> &color);

int getres (int i, int j, const Header &h, const std::vector<uint8_t> &color);

Image gauss (const Image &img, const Header &h);

void histogram(const Image &img, std::filesystem::path path);

void call_histogram(const Image &image, const std::filesystem::path &new_file);

Image mono(Image &image);

Image perform_op(Image image, std::string &op, const std::filesystem::path &new_file, const Header &header);

#endif//CA_P1_SOA_HPP
