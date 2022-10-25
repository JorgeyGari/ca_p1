/* Source file containing functions exclusive to the AOS version */

#include "aos.hpp"
#include "common_rw.cpp"
#include "common_hst.cpp"
#include <iostream>
#include <filesystem>

std::vector<struct Pixel> read_pixels(std::filesystem::path &path, uint32_t start, uint32_t width, uint32_t height)
// Reads the RGB values of each pixel in the image
{
    std::ifstream f;
    f.open(path, std::ios::in | std::ios::binary);
    /* We do not need to check if it exists or if it could be opened because read_header() already did */

    f.seekg(start);

    int px = int(width * height);
    std::vector<Pixel> img(px);

    /* To calculate how many bytes of padding we have in each row, first we find out how many bytes the colors take up:
     * that is three bytes per color. That amount modulo 4 tells us how many bytes over we are, and 4 minus that amount
     * gives us how many bytes we have left. If the color bytes modulo 4 is 0, then that amount is 4, and we may skip
     * information from the image, so we apply modulo 4 once again to convert that to 0.
    */
    const int padding_bytes = (4 - (int(width) * 3) % 4) % 4;

    for (int i = 0; i < px; i++) {
        f.read(reinterpret_cast<char *>(&img[i].b), sizeof(uint8_t));
        f.read(reinterpret_cast<char *>(&img[i].g), sizeof(uint8_t));
        f.read(reinterpret_cast<char *>(&img[i].r), sizeof(uint8_t));
        if (i % width == 0) {
            f.ignore(padding_bytes);
        }
    }

    return img;
}

void write_bmp(std::filesystem::path &path, const Header& header, std::vector<Pixel> image)
// Writes a (valid) bitmap file in the specified directory using a given header and the color values for its pixels
{
    write_header(path, header);

    std::ofstream f;
    f.open(path, std::ios::in | std::ios::binary);
    /* We do not need to check for errors because write_header() already did */

    f.seekp(int(header.img_start));

    const int padding_bytes = (4 - (static_cast<int>(header.img_width) * 3) % 4) % 4;
    int px = int(header.img_width * header.img_height);
    int zero = 0;   // FIXME: This is stupid
    for (int i = 0; i < px; i++) {
        f.write(reinterpret_cast<char *>(&image[i].b), sizeof(uint8_t));
        f.write(reinterpret_cast<char *>(&image[i].g), sizeof(uint8_t));
        f.write(reinterpret_cast<char *>(&image[i].r), sizeof(uint8_t));
        if (i % header.img_width == 0) {
            f.write(reinterpret_cast<char *>(&zero), padding_bytes);
        }
    }
}

void count (std::vector<int> &freq_red, std::vector<int> &freq_green, std::vector<int> &freq_blue, const std::vector<Pixel> &img) {
    int n = static_cast<int>(img.size()); // Save the number of pixels in the image
    for (int i = 0; i < n; i++) {
        freq_red[img[i].r]++;
        freq_green[img[i].g]++;
        freq_blue[img[i].b]++;
    }
}

void histogram (const std::vector<Pixel> &img) {
    std::filesystem::path path = "./aos.hst";
    std::ofstream f = open_file(path);

    // Declare 3 vectors to count the frequencies, one for each color
    std::vector<int> freq_red, freq_green, freq_blue;
    freq_red.resize(256); // size = 256 as we can have those values
    freq_green.resize(256);
    freq_blue.resize(256);

    count(freq_red, freq_green, freq_blue, img);

    print_to_file(freq_red, f);
    print_to_file(freq_green, f);
    print_to_file(freq_blue, f);
}
