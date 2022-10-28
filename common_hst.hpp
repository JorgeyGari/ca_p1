/* Header containing definitions common to the AOS and SOA histogram methods */

#ifndef IMAGE_COMMON_HST_HPP
#define IMAGE_COMMON_HST_HPP

#include "common.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>

std::ofstream open_file(std::filesystem::path &path);

void print_to_file(std::vector<int> &freq, std::ofstream &f);

#endif//IMAGE_COMMON_HST_HPP
