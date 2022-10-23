#include "common.hpp"
#include <iostream>
#include <fstream>

void err_msg(ErrorType error)
// Receives a member of the ErrorType class and displays its corresponding error message
{
    switch (error) {
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

void read_type(std::ifstream &f)
// Reads and checks that the encoded filetype is a bitmap
{
    uint8_t file_type_B;
    uint8_t file_type_M;

    f.read(reinterpret_cast<char *>(&file_type_B), sizeof(unsigned char));   // The first 2 bytes tell us the file type, which should be BM
    if (file_type_B != 'B') // Check that the file type is correct
    {
        err_msg(ErrorType::wrong_type);
    }

    f.read(reinterpret_cast<char *>(&file_type_M), sizeof(unsigned char));
    if (file_type_M != 'M')
    {
        err_msg(ErrorType::wrong_type);
    }
}

void check_validity(std::ifstream &f)
// Checks if the bitmap is valid
{
    /* For a bitmap to be considered valid: the number of plains must be 1, the point size must be 24
     * and the compression must be 0.
     * All the necessary fields are next to each other, so we will store them in the same array.
    */

    uint16_t planes, point_size;
    f.read(reinterpret_cast<char *>(&planes), 2);

    if (static_cast<int>(planes) != 1) // Check number of planes
    {
        err_msg(ErrorType::wrong_planes);
    }

    f.read(reinterpret_cast<char *>(&point_size), 2);

    if (static_cast<int>(point_size) != 24)    // Check point size
    {
        err_msg(ErrorType::wrong_point_size);
    }

    uint32_t compression;
    f.read(reinterpret_cast<char *>(&compression), sizeof(unsigned int));

    if (static_cast<int>(compression) != 0) // Check compression
    {
        err_msg(ErrorType::wrong_compression);
    }
}

Header read_header(const std::filesystem::path &path)
// Reads the values in the header of a valid bitmap and returns them in a structure
{
    Header h{};
    std::ifstream f; // Create a file stream;
    f.open(path, std::ios::in | std::ios::binary); // Open the file in the specified path as input and read it in binary

    if (!f.is_open())   // Check if the file could be opened correctly
    {
        err_msg(ErrorType::unopened_file);    // If not, we will output an error message determined by err_msg()
    }

    read_type(f);   // Checking the file type

    f.read(reinterpret_cast<char *>(&h.file_size), sizeof(unsigned int));

    f.ignore(4);   // Skip the reserved field

    f.read(reinterpret_cast<char *>(&h.img_start), sizeof(unsigned int));
    f.read(reinterpret_cast<char *>(&h.header_size), sizeof(unsigned int));
    f.read(reinterpret_cast<char *>(&h.img_width), sizeof(unsigned int));
    f.read(reinterpret_cast<char *>(&h.img_height), sizeof(unsigned int));

    check_validity(f);  // Checking the validity of the bitmap

    // FIXME: These values are not read correctly (from h_res onwards, they are all read as 0)
    f.read(reinterpret_cast<char *>(&h.image_size), sizeof(unsigned int));
    f.read(reinterpret_cast<char *>(&h.h_res), sizeof(unsigned int));
    f.read(reinterpret_cast<char *>(&h.v_res), sizeof(unsigned int));
    f.read(reinterpret_cast<char *>(&h.ctable_size), sizeof(unsigned int));
    f.read(reinterpret_cast<char *>(&h.ccounter), sizeof(unsigned int));

    return h;
}

void write_header(std::filesystem::path &path, Header header)
// Writes a (valid) bitmap header in the specified directory using a given header structure
{
    std::ofstream f;    // Create an output file stream
    f.open(path, std::ios::out |
                 std::ios::binary); // Open the file in the specified path as an output to write binary values to

    if (!f.is_open())   // Check if the file could be opened correctly
    {
        err_msg(ErrorType::unopened_file);    // If not, we will output an error message determined by err_msg()
    }

    /* Write the file type */
    unsigned char file_type[2];
    file_type[0] = 'B';
    file_type[1] = 'M';
    f.write(reinterpret_cast<const char *>(&file_type[0]), sizeof(file_type[0]));
    f.write(reinterpret_cast<const char *>(&file_type[1]), sizeof(file_type[1]));

    f.write(reinterpret_cast<const char *>(&header.file_size), sizeof(unsigned int));

    f.seekp(10);    // Skip the reserved field

    f.write(reinterpret_cast<const char *>(&header.img_start), sizeof(unsigned int));
    f.write(reinterpret_cast<const char *>(&header.header_size), sizeof(unsigned int));
    f.write(reinterpret_cast<const char *>(&header.img_width), sizeof(unsigned int));
    f.write(reinterpret_cast<const char *>(&header.img_height), sizeof(unsigned int));

    /* Since the output file has to be valid, we can assume the next three fields */
    int valid_values[] = {1, 24, 0};
    f.write(reinterpret_cast<const char *>(&valid_values[0]), sizeof(unsigned int));   // Number of plains: 1
    f.write(reinterpret_cast<const char *>(&valid_values[1]), sizeof(unsigned int));  // Point size in bits: 24
    f.write(reinterpret_cast<const char *>(&valid_values[2]), sizeof(unsigned int)); // Compression: 0

    f.write(reinterpret_cast<const char *>(&header.image_size), sizeof(unsigned int));
    f.write(reinterpret_cast<const char *>(&header.h_res), sizeof(unsigned int));
    f.write(reinterpret_cast<const char *>(&header.v_res), sizeof(unsigned int));
    f.write(reinterpret_cast<const char *>(&header.ctable_size), sizeof(unsigned int));
    f.write(reinterpret_cast<const char *>(&header.ccounter), sizeof(unsigned int));
}
