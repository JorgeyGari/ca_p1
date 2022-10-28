/* Header containing definitions exclusive to the AOS version */

#ifndef CA_P1_AOS_HPP
#define CA_P1_AOS_HPP

#include "common_rw.hpp"
#include <filesystem>
#include <vector>

struct Pixel
// Structure that the AOS will hold, represents one pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

std::vector<struct Pixel> read_pixels(const std::filesystem::path &path, uint32_t start, uint32_t width, uint32_t height);

void write_bmp(std::filesystem::path &path, const Header &header, std::vector<Pixel> image);

std::vector<Pixel> perform_op(std::vector<Pixel> image, std::string &op, const std::filesystem::path& new_file, const Header &header);

void count(std::vector<int> &freq_red, std::vector<int> &freq_green, std::vector<int> &freq_blue, const std::vector<Pixel> &img);

void histogram(const std::vector<Pixel> &img, std::filesystem::path path);

Pixel getim(int position, const std::vector<Pixel> &img);

int multiply(uint8_t color, int m);

Pixel pixel_to_zero(Pixel p);

Pixel getres(int sumatoryr, int sumatoryg, int sumatoryb);

Pixel getmim(int i, int j, const std::vector<Pixel> &img, const Header &h);

std::vector<Pixel> gauss(const std::vector<Pixel> &img, const Header &h);

void call_histogram(const std::vector<Pixel> &image, const std::filesystem::path &new_file);

std::vector<Pixel> mono(std::vector<Pixel> &image);

#endif//CA_P1_AOS_HPP
