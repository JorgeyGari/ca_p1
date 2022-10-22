#include "common.hpp"

#include <iostream>
#include <fstream>

void err_msg(ErrorType error)   // Receives a member of the ErrorType class and displays its corresponding error message
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

Header read_header(const std::filesystem::path &path)   // Reads the values in the header of a valid bitmap and returns them in a structure
{
    std::ifstream f; // Create a file stream;
    f.open(path, std::ios::in | std::ios::binary); // Open the file in the specified path as input and read it in binary

    if (!f.is_open())   // Check if the file could be opened correctly
    {
        err_msg(ErrorType::unopened_file);    // If not, we will output an error message determined by err_msg()
    }

    /* Reading the file type */
    read_type(f);

    uint32_t file_size;
    f.read(reinterpret_cast<char *>(&file_size), sizeof(unsigned int));

    /* Now we declare some variables holding the amount of unsigned bytes required for each of the fields we need. */
    uint32_t start, header_size, width, height;

    f.ignore(4);   // Skip the reserved field

    /* Reading the start position of the image data */
    f.read(reinterpret_cast<char *>(&start),
           sizeof(unsigned int)); // We will need to interpret an integer from these bytes

    f.read(reinterpret_cast<char *>(&header_size), sizeof(unsigned int));

    /* Reading the image's width and height in pixels */
    f.read(reinterpret_cast<char *>(&width), sizeof(unsigned int));
    f.read(reinterpret_cast<char *>(&height), sizeof(unsigned int));

    /* Checking the validity of our bitmap file. */
    check_validity(f);

    /* Finally, we need to copy the rest of the header for the output image */
    uint32_t image_size, h_res, v_res, ctable_size, ccounter;   // Declare a variable for each field

    f.read(reinterpret_cast<char *>(&image_size), sizeof(unsigned int));
    f.read(reinterpret_cast<char *>(&h_res), sizeof(unsigned int));
    f.read(reinterpret_cast<char *>(&v_res), sizeof(unsigned int));
    f.read(reinterpret_cast<char *>(&ctable_size), sizeof(unsigned int));
    f.read(reinterpret_cast<char *>(&ccounter), sizeof(unsigned int));

    /* We can return the non-fixed values of the header as a structure */
    Header h{start, header_size, width, height, image_size, h_res, v_res, ctable_size, ccounter};

    return h;
}
