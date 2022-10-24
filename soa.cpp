//
// Created by laura on 23/10/2022.
//

#include "soa.hpp"
#include "common_hst.cpp"

#include <iostream>
#include <filesystem>

Image read_pixels(std::filesystem::path &path, uint32_t start, uint32_t width, uint32_t height)
// Reads the RGB values of each pixel in the image
{
    std::ifstream f; // Create a file stream
    f.open(path, std::ios::in | std::ios::binary); // Open the file in the specified path as input and read it in binary
    /* We do not need to check if it exists or if it could be opened because read_header() already did */

    f.seekg(start); // Go to the address where the image data starts

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
    }

    /* Due to the way information is encoded in BMP files, the pixels are stored by rows from bottom to top.
     * That is: the first pixel that we read is actually the bottom left corner of the image, and the last one is the
     * top right corner.
     * Also, due to the format of the file, the order of the colors is inverted, hence we read blue first and red last.
    */

    /* Finally, we declare the structure of arrays and return it. */
    Image img{red, green, blue};
    return img;
}

void write_bmp(std::filesystem::path &path, Header header, Image image)
// Writes a (valid) bitmap file in the specified directory using a given header and the color values for its pixels
{
    write_header(path, header);

    /* Once the header is written, we can start setting the color values of each pixel */
    std::ofstream f;    // Create a file output stream
    f.open(path, std::ios::in | std::ios::binary);  // Open the file in the specified path, we do not need to check for errors because write_header() already did

    if (!f.is_open()) {
        err_msg(ErrorType::unopened_file);
    }

    f.seekp(int(header.img_start)); // Seek the position where the image data starts
    const int padding_bytes = ((4 - (int(header.img_width) * 3)) % 4) % 4;  // Calculate padding bytes as explained in read_pixels()
    int px = int(header.img_width * header.img_height);
    int zero = 0;
    for (int i = 0; i < px; i++) {
        f.write(reinterpret_cast<char *>(&image.b[i]), sizeof(uint8_t));
        f.write(reinterpret_cast<char *>(&image.g[i]), sizeof(uint8_t));
        f.write(reinterpret_cast<char *>(&image.r[i]), sizeof(uint8_t));
        f.write(reinterpret_cast<char *>(&zero), padding_bytes);
    }
}

void frequencies (const std::vector<uint8_t> &color, std::ofstream& f) {
    std::vector<int> freq; // Declare a vector to count the occurrences of each value (from 0 to 255) of the color in each pixel
    freq.resize(256);
    for (uint8_t element: color) {
        // Every time a value appears, we add 1 to the element in the vector of frequencies that represents that value
        freq[element]++;
    }
    print_to_file(freq, f);
}

void histogram (const Image &img) {
    std::filesystem::path path = "./soa.hst";
    std::ofstream f = open_file(path);

    frequencies(img.r, f);
    frequencies(img.g, f);
    frequencies(img.b, f);
}

std::vector<int> gaussexpression (int m[5][5], int w, Header h, std::vector<uint8_t> color) {
    std::vector<int> res;
    for (int i = 0; i < static_cast<int>(h.img_height); i++) {
        for (int j = 0; j < static_cast<int>(h.img_width); j++) {
            int mult;
            for (int s = -3; s < 2; s++) {
                for (int t = -3; t < 2; t++) {
                    mult += m[s + 3][t + 3] * color[((i+s)*static_cast<int>(h.img_width)) + (j+t)];
                }
            }

        }
    }
    return res;
}

Image gauss (const Image &img, const Header &h) {
    Image res = img;
    int m[5][5] = {{1, 4, 7, 4, 1}, {4, 16, 26, 16, 4}, {7, 26, 41, 26, 7}, {4, 16, 26, 16, 4}, {1, 4, 7, 4, 1}}, w = 273;

    printf("m(0, 0): %d\n", m[0][0]);
    printf("w = %d\n", w);
    printf("h = %d\n", h.img_height);
    printf("width = %d\n", h.img_width);
    printf("size = %d", h.img_height*h.img_width);

    return res;
}