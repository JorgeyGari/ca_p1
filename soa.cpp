#include "soa.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

void err_msg(ErrorType e)   // Receives a member of the ErrorType class and displays its corresponding error message
{
    switch (e) {
        case ErrorType::unopened_file:
            std::cout << "The specified file does not exist or could not be opened.\n";
            break;
        case ErrorType::wrong_type:
            std::cout << "The specified file is not a bitmap image.\n";
            break;
        case ErrorType::wrong_planes:
            std::cout << "The specified bitmap is invalid (number of plains is not equal to 1).\n";
            break;
        case ErrorType::wrong_point_size:
            std::cout << "The specified bitmap is invalid (point size is not equal to 24).\n";
            break;
        case ErrorType::wrong_compression:
            std::cout << "The specified bitmap is invalid (compression value is not equal to 0).\n";
            break;
    }
    exit(-1);   // Finish the execution after the error message is displayed with error code -1
}

void read_type(std::ifstream &f)    // Reads and checks that the encoded filetype is a bitmap
{
    uint8_t file_type[2];   // We need to check two characters
    f.read(reinterpret_cast<char *>(file_type),
           sizeof(file_type));   // The first 2 bytes tell us the file type, which should be BM

    if (file_type[0] != 'B' || file_type[1] != 'M') // Check that the file type is correct
    {
        err_msg(ErrorType::wrong_type);
    }
}

void check_validity(std::ifstream &f)   // Checks if the bitmap is valid
{
    /* For a bitmap to be considered valid: the number of plains must be 1, the point size must be 24
     * and the compression must be 0.
     * All the necessary fields are next to each other, so we will store them in the same array.
    */

    uint16_t validity[3];

    f.read(reinterpret_cast<char *>(&validity), sizeof(validity));   // We read a total of three integers

    if (static_cast<int>(validity[0]) != 1) // Check number of plains
    {
        err_msg(ErrorType::wrong_planes);
    }

    if (static_cast<int>(validity[1]) != 24)    // Check point size
    {
        err_msg(ErrorType::wrong_point_size);
    }

    if (static_cast<int>(validity[2]) != 0) // Check compression
    {
        err_msg(ErrorType::wrong_compression);
    }
}

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

Image read_bmp(std::filesystem::path p) {
    std::ifstream f; // Create a file stream;
    f.open(p, std::ios::in | std::ios::binary); // Open the file in the specified path as input and read it in binary

    if (!f.is_open())   // Check if the file could be opened correctly
    {
        err_msg(ErrorType::unopened_file);    // If not, we will output an error message determined by err_msg()
    }

    read_type(f);

    /* Now we declare some variables holding the amount of unsigned bytes required for each of the fields we need. */
    uint32_t start, width, height;

    /* Reading the file type */

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
