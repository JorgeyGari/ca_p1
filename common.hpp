#ifndef IMAGE_COMMON_HPP
#define IMAGE_COMMON_HPP

#include <string>
#include <chrono>

enum class ErrorType// Possible errors while reading, used by function err_msg()
{
    unopened_file,
    wrong_type,
    wrong_planes,
    wrong_point_size,
    wrong_compression
};

void err_msg(ErrorType error);

void print_data(const std::string &op, long loadtime, long opertime, long storetime);

long stop_chrono(std::chrono::time_point<std::chrono::system_clock> start);

#endif//IMAGE_COMMON_HPP
