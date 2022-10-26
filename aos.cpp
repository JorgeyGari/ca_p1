/* Source file containing functions exclusive to the AOS version */

#include "aos.hpp"
#include "common_gauss.hpp"
#include "common_hst.hpp"
#include "common_rw.hpp"
#include <filesystem>
#include <iostream>
#include <cstring>

std::vector<struct Pixel> read_pixels(const std::filesystem::path &path, uint32_t start, uint32_t width, uint32_t height)
// Reads the RGB values of each pixel in the image
{
    std::ifstream f;
    f.open(path, std::ios::in | std::ios::binary);
    /* We do not need to check if it exists or if it could be opened because read_header() already did */

    f.seekg(start);

    int px = int(width * height);
    std::vector<Pixel> img(px);

    /* To calculate how many bytes of padding we have in each row, first we find out how many bytes the colors take up:
     * that is three bytes per color. That amount modulo 4 tells us how many bytes over we are, and 4 minus that amount
     * gives us how many bytes we have left. If the color bytes modulo 4 is 0, then that amount is 4, and we may skip
     * information from the image, so we apply modulo 4 once again to convert that to 0.
    */
    const int padding_bytes = (4 - (int(width) * 3) % 4) % 4;
    int read = 0;

    for (int i = 0; i < px; i++) {
        f.read(reinterpret_cast<char *>(&img[i].b), sizeof(uint8_t));
        read++;
        f.read(reinterpret_cast<char *>(&img[i].g), sizeof(uint8_t));
        read++;
        f.read(reinterpret_cast<char *>(&img[i].r), sizeof(uint8_t));
        read++;
        if (read == static_cast<int>(width) * 3) {
            read = 0;
            f.ignore(padding_bytes);
        }
    }

    return img;
}

void write_bmp(std::filesystem::path &path, const Header& header, std::vector<Pixel> image)
// Writes a (valid) bitmap file in the specified directory using a given header and the color values for its pixels
{
    write_header(path, header);

    std::ofstream f;
    f.open(path, std::ios::in | std::ios::binary);
    /* We do not need to check for errors because write_header() already did */

    f.seekp(int(header.img_start));

    const int padding_bytes = (4 - (static_cast<int>(header.img_width) * 3) % 4) % 4;
    int px = int(header.img_width * header.img_height);
    int zero = 0;// FIXME: This is stupid
    int wrote = 0;
    for (int i = 0; i < px; i++) {
        f.write(reinterpret_cast<char *>(&image[i].b), sizeof(uint8_t));
        wrote++;
        f.write(reinterpret_cast<char *>(&image[i].g), sizeof(uint8_t));
        wrote++;
        f.write(reinterpret_cast<char *>(&image[i].r), sizeof(uint8_t));
        wrote++;
        if (wrote == static_cast<int>(header.img_width) * 3) {
            f.write(reinterpret_cast<const char *>(&zero), padding_bytes);
            wrote = 0;
        }
    }
}

void count(std::vector<int> &freq_red, std::vector<int> &freq_green, std::vector<int> &freq_blue, const std::vector<Pixel> &img) {
    int n = static_cast<int>(img.size());// Save the number of pixels in the image
    for (int i = 0; i < n; i++) {
        freq_red[img[i].r]++;
        freq_green[img[i].g]++;
        freq_blue[img[i].b]++;
    }
}

void histogram(const std::vector<Pixel> &img, std::filesystem::path path) {
    std::ofstream f = open_file(path);

    // Declare 3 vectors to count the frequencies, one for each color
    std::vector<int> freq_red, freq_green, freq_blue;
    freq_red.resize(256);// size = 256 as we can have those values
    freq_green.resize(256);
    freq_blue.resize(256);

    count(freq_red, freq_green, freq_blue, img);

    print_to_file(freq_red, f);
    print_to_file(freq_green, f);
    print_to_file(freq_blue, f);
}

Pixel getim(int position, const std::vector<Pixel> &img) {
    Pixel p{};
    p.r = img[position].r;
    p.g = img[position].g;
    p.b = img[position].b;
    return p;
}

int multiply(uint8_t color, int m) {
    int ip = static_cast<int>(color);
    int mip = m * ip;
    return mip;
}

Pixel pixel_to_zero(Pixel p) {
    p.r = static_cast<uint8_t>(0);
    p.g = static_cast<uint8_t>(0);
    p.b = static_cast<uint8_t>(0);
    return p;
}

Pixel getres(int sumatoryr, int sumatoryg, int sumatoryb) {
    Pixel res{};
    int w = 273;

    int resr = sumatoryr / w;
    int resg = sumatoryg / w;
    int resb = sumatoryb / w;

    res.r = static_cast<uint8_t>(resr);
    res.g = static_cast<uint8_t>(resg);
    res.b = static_cast<uint8_t>(resb);

    return res;
}

Pixel getmim(int i, int j, const std::vector<Pixel> &img, const Header &h) {
    int sumatoryr = 0, sumatoryg = 0, sumatoryb = 0;
    for (int s = -3; s < 2; s++) {
        for (int t = -3; t < 2; t++) {
            Pixel p{};
            int position = ((i + s) * static_cast<int>(h.img_width)) + (j + t);
            if (i + s < 0 || j + t < 0 || i + s > static_cast<int>(h.img_height) - 1 || j + t > static_cast<int>(h.img_width) - 1) {
                p = pixel_to_zero(p);
            } else {
                p = getim(position, img);
            }
            int m = getm(s, t);
            int mimr = multiply(p.r, m);
            int mimg = multiply(p.g, m);
            int mimb = multiply(p.b, m);
            sumatoryr += mimr;
            sumatoryg += mimg;
            sumatoryb += mimb;
        }
    }
    Pixel res = getres(sumatoryr, sumatoryg, sumatoryb);
    return res;
}

std::vector<Pixel> gauss(const std::vector<Pixel> &img, const Header &h) {
    std::vector<Pixel> res;
    res.resize(h.img_height * h.img_width);
    for (int i = 0; i < static_cast<int>(h.img_height); i++) {
        for (int j = 0; j < static_cast<int>(h.img_width); j++) {
            Pixel resij = getmim(i, j, img, h);
            res[i * h.img_width + j] = resij;
        }
    }
    return res;
}

void perform_op(const std::vector<Pixel> &image, std::string &op, std::filesystem::path new_file, const Header &header) {
    const char *string = op.c_str();
    if (strcmp(string, "copy") == 0) {
        write_bmp(new_file, header, image);
    } else if (strcmp(string, "histo") == 0) {
        histogram(image);
        write_bmp(new_file, header, image);
    } else if (strcmp(string, "mono") == 0) {
        std::cout << "mono is not yet implemented, no modifications will be made to the images\n";
        write_bmp(new_file, header, image);
    } else {
        gauss(image, header);
        write_bmp(new_file, header, image);
    }
}
