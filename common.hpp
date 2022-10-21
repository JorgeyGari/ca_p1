#ifndef CA_P1_COMMON_HPP
#define CA_P1_COMMON_HPP

#include <vector>
#include <filesystem>

enum class ErrorType    // Possible errors while reading, used by function err_msg()
{
    unopened_file, wrong_type, wrong_planes, wrong_point_size, wrong_compression
};

#endif //CA_P1_COMMON_HPP
