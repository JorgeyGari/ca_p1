/* Source file containing functions exclusive to the AOS version */

#include "aos.hpp"
#include "common_rw.cpp"
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
    const int padding_bytes = ((4 - (int(width) * 3)) % 4) % 4;

    for (int i = 0; i < px; i++) {
        f.read(reinterpret_cast<char *>(&img[i].b), sizeof(uint8_t));
        f.read(reinterpret_cast<char *>(&img[i].g), sizeof(uint8_t));
        f.read(reinterpret_cast<char *>(&img[i].r), sizeof(uint8_t));
        f.ignore(padding_bytes);
    }

    return img;
}

void write_bmp(std::filesystem::path &path, Header header, std::vector<Pixel> image)
// Writes a (valid) bitmap file in the specified directory using a given header and the color values for its pixels
{
    write_header(path, header);

    std::ofstream f;
    f.open(path, std::ios::in | std::ios::binary);
    /* We do not need to check for errors because write_header() already did */

    f.seekp(int(header.img_start));

    const int padding_bytes = ((4 - (int(header.img_width) * 3)) % 4) % 4;

    int px = int(header.img_width * header.img_height);
    for (int i = 0; i < px; i++) {
        f.write(reinterpret_cast<char *>(&image[i].b), sizeof(uint8_t));
        f.write(reinterpret_cast<char *>(&image[i].g), sizeof(uint8_t));
        f.write(reinterpret_cast<char *>(&image[i].r), sizeof(uint8_t));
        f.write(nullptr, padding_bytes);
    }
}
