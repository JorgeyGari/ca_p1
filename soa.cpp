#include "soa.hpp"
#include "common_hst.cpp"

#include <iostream>
#include <filesystem>

Image read_pixels(std::ifstream &f, uint32_t start, uint32_t width,
                  uint32_t height) // Reads the RGB values of each pixel in the image
{
    f.seekg(start); // Go to the address where the image data starts, which we had read before

    int px = int(width * height);
    std::vector<uint8_t> blue(px);
    std::vector<uint8_t> green(px);
    std::vector<uint8_t> red(px);

    /* To calculate how many bytes of padding we have in each row, first we find out how many bytes the colors take up:
     * that is three bytes per color. That amount modulo 4 tells us how many bytes over we are, and 4 minus that amount
     * gives us how many bytes we have left. If the color bytes modulo 4 is 0, then that amount is 4, and we may skip
     * information from the image, so we apply modulo 4 once again to convert that to 0.
    */
    const int padding_bytes = ((4 - (int(width) * 3)) % 4) % 4;

    for (int i = 0; i < px; i++) {
        f.read(reinterpret_cast<char *>(&blue[i]), sizeof(uint8_t));
        f.read(reinterpret_cast<char *>(&green[i]), sizeof(uint8_t));
        f.read(reinterpret_cast<char *>(&red[i]), sizeof(uint8_t));
        f.ignore(padding_bytes);
        // std::cout << "Pixel #" << i << ": " << int(red[i]) << " " << int(green[i]) << " " << int(blue[i]) << "\n";
    }

    /* Due to the way information is encoded in BMP files, the pixels are stored by rows from bottom to top.
     * That is: the first pixel that we read is actually the bottom left corner of the image, and the last one is the
     * top right corner.
     * Also, due to the format of the file, the order of the colors is inverted, hence we read blue first and red last.
    */

    /* Finally, we declare the structure of arrays and return it. */
    Image img;
    img.r = red;
    img.g = green;
    img.b = blue;

    return img;
}

Image read_bmp(const std::filesystem::path &path) {
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
    Image img = read_pixels(f, start, width, height);

    return img;
}

void frequencies (const std::vector<uint8_t> &color, std::ofstream& f) {
    std::vector<int> freq; // Declare a vector to count the occurrences of each value (from 0 to 255) of the color in each pixel
    freq.resize(256);
    for (uint8_t element: color) {
        // Every time a value appears, we add 1 to the element in the vector of frequencies that represents that value
        freq[element]++;
    }
    for (int i = 0; i < 256; i++) {
        f << freq[i] << "\n";
    }
}

void histogram (const Image &img) {
    std::filesystem::path path = "./soa.hst";
    std::ofstream f = open_file(path);

    frequencies(img.r, f);
    frequencies(img.g, f);
    frequencies(img.b, f);
}
