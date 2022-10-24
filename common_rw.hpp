#ifndef CA_P1_COMMON_HPP
#define CA_P1_COMMON_HPP

#include <vector>
#include <filesystem>

struct Header   // A structure holding the relevant header values of a valid bitmap
{
    uint32_t file_size, img_start, header_size, img_width, img_height, image_size, h_res, v_res, ctable_size, ccounter;
};

#endif //CA_P1_COMMON_HPP
