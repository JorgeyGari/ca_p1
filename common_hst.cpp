/* Source file containing common functions for SOA and AOS histogram methods */

#include "common.hpp"
#include "common_hst.hpp"
#include <vector>
#include <iostream>
#include <filesystem>

std::ofstream open_file(std::filesystem::path &path) {
    std::ofstream f;
    f.open(path, std::ios::out);// Open the file for writing
    if (!f.is_open()) {         // Check for errors
        err_msg(ErrorType::unopened_file);
    }
    return f;
}

void print_to_file(std::vector<int> &freq, std::ofstream &f) {
    for (int i = 0; i < 256; i++) {
        f << freq[i] << "\n";
    }
}
