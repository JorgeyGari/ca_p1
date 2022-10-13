#include "soa.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

void read_bmp(std::filesystem::path p) {
    std::ifstream f; // Create a file stream;
    f.open(p, std::ios::in | std::ios::binary); // Open the file in the specified path as input and read it in binary

    if (!f.is_open()) { // Check if the file could be opened correctly
        f.close(); // If not, close the file...
        std::cout << "The specified file could not be opened.\n"; // ...print out an error message...
        return; // ...and finish the execution
    }

    unsigned char file_header[2];

    f.read(reinterpret_cast<char *>(file_header), 2);

    if (file_header[0] != 'B' || file_header[1] != 'M') {    // Check that the file type is correct
        f.close();  // If not, then we close the file...
        std::cout << "The specified file is not a bitmap image.";   // ...output an error message...
        return; // ...and finish the execution
    }

    /* Now we need to read the other fields... but I don't know how to check their values in this format. */


    f.close();
}
