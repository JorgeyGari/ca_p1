#ifndef IMAGE_COMMON_HPP
#define IMAGE_COMMON_HPP

enum class ErrorType// Possible errors while reading, used by function err_msg()
{
    unopened_file,
    wrong_type,
    wrong_planes,
    wrong_point_size,
    wrong_compression
};

void err_msg(ErrorType error);

#endif//IMAGE_COMMON_HPP
