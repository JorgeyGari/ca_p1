#include "soa.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

void read_bmp(std::filesystem::path p) {
    std::ifstream f; // Create a file stream;
    f.open(p, std::ios::in | std::ios::binary); // Open the file in the specified path as input and read it in binary

    if (!f.is_open())   // Check if the file could be opened correctly
    {
        f.close(); // If not, close the file...
        std::cout << "The specified file could not be opened.\n"; // ...print out an error message...
        return; // ...and finish the execution
    }

    unsigned char file_header[2], validity[6];

    f.read(reinterpret_cast<char *>(file_header), 2);

    if (file_header[0] != 'B' || file_header[1] != 'M') // Check that the file type is correct
    {
        f.close();  // If not, then we close the file...
        std::cout << "The specified file is not a bitmap image.";   // ...output an error message...
        return; // ...and finish the execution
    }

    f.ignore(24);   // We do not need any of the fields that take up the next 24 bytes

    /* Once we know we are working with a bitmap file, we have to check if it is valid.
     * For a bitmap to be considered valid: the number of plains must be 1, the point size must be 24
     * and the compression must be 0.
     * All the necessary fields are next to each other, so we will store them in the same array.
    */

    f.read(reinterpret_cast<char *>(validity), 6);

    if (static_cast<int>(validity[0]) != 1) // Check number of plains
    {
        f.close();
        std::cout << "The specified bitmap is invalid (number of plains is not equal to 1).";
        return;
    }

    if (static_cast<int>(validity[2]) != 24)    // Check point size
    {
        f.close();
        std::cout << "The specified bitmap is invalid (point size is not equal to 24).";
        return;
    }

    if (static_cast<int>(validity[4]) != 0) // Check compression
    {
        f.close();
        std::cout << "The specified bitmap is invalid (compression value is not equal to 0).";
        return;
    }

    f.close();  // Close the file once we are done reading it
}
