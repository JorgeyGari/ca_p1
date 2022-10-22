#ifndef CA_P1_COMMON_HPP
#define CA_P1_COMMON_HPP

#include <vector>
#include <filesystem>

enum class ErrorType    // Possible errors while reading, used by function err_msg()
{
    unopened_file, wrong_type, wrong_planes, wrong_point_size, wrong_compression
};

struct Header   // A structure holding the relevant header values of a valid bitmap
{
    uint32_t file_size, img_start, header_size, img_width, img_height, image_size, h_res, v_res, ctable_size, ccounter;
};

#endif //CA_P1_COMMON_HPP
