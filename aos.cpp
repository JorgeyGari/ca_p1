#include "aos.hpp"
#include "common.cpp"
#include "common.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

std::vector<struct Pixel> read_pixels(std::ifstream &f, uint32_t start, uint32_t width,
                                      uint32_t height) // Reads the RGB values of each pixel in the image
{
    f.seekg(start); // Go to the address where the image data starts, which we had read before

    int px = int(width * height);
    std::vector<Pixel> img(px);

    /* To calculate how many bytes of padding we have in each row, first we find out how many bytes the colors take up:
     * that is three bytes per color. That amount modulo 4 tells us how many bytes over we are, and 4 minus that amount
     * gives us how many bytes we have left. If the color bytes modulo 4 is 0, then that amount is 4, and we may skip
     * information from the image, so we apply modulo 4 once again to convert that to 0.
    */
    const int padding_bytes = ((4 - (int(width) * 3)) % 4) % 4;

    for (int i = 0; i < px; i++) {
        f.read(reinterpret_cast<char *>(img[i].b), sizeof(uint8_t));
        f.read(reinterpret_cast<char *>(&img[i].g), sizeof(uint8_t));
        f.read(reinterpret_cast<char *>(&img[i].r), sizeof(uint8_t));
        f.ignore(padding_bytes);
    }

    /* Due to the way information is encoded in BMP files, the pixels are stored by rows from bottom to top.
     * That is: the first pixel that we read is actually the bottom left corner of the image, and the last one is the
     * top right corner.
     * Also, due to the format of the file, the order of the colors is inverted, hence we read blue first and red last.
    */

    return img;
}

std::vector<struct Pixel> read_bmp(const std::filesystem::path &path) {
    std::ifstream f; // Create a file stream;
    f.open(path, std::ios::in | std::ios::binary); // Open the file in the specified path as input and read it in binary

    if (!f.is_open())   // Check if the file could be opened correctly
    {
        err_msg(ErrorType::unopened_file);    // If not, we will output an error message determined by err_msg()
    }

    /* Reading the file type */
    read_type(f);

    /* Now we declare some variables holding the amount of unsigned bytes required for each of the fields we need. */
    uint32_t start, width, height;

    f.ignore(8);   // We do not need any of the fields that take up the next 8 bytes

    /* Reading the start position of the image data */
    f.read(reinterpret_cast<char *>(&start),
           sizeof(unsigned int)); // We will need to interpret an integer from these bytes

    f.ignore(4);    // Next 4 bytes reserved for storing the size of the bitmap header, which we do not need

    /* Reading the image's width and height in pixels */
    f.read(reinterpret_cast<char *>(&width), sizeof(width));
    f.read(reinterpret_cast<char *>(&height), sizeof(height));

    /* Checking the validity of our bitmap file. */
    check_validity(f);

    /* Now we can start reading the image's pixels. */
    std::vector<Pixel> img = read_pixels(f, start, width, height);

    return img;
}
