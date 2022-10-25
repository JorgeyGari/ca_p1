#ifndef CA_P1_COMMON_HPP
#define CA_P1_COMMON_HPP

#include <vector>
#include <filesystem>

struct Header
// A structure holding the relevant header values of a valid bitmap
{
    uint32_t file_size, img_start, header_size, img_width, img_height, image_size, h_res, v_res, ctable_size, ccounter;
    std::vector<uint8_t> header;
};

void read_type(std::ifstream &f);

void check_validity(std::ifstream &f);

Header read_header(const std::filesystem::path &path);

void write_header(std::filesystem::path &path, Header header);

#endif //CA_P1_COMMON_HPP
