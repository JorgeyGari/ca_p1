/* Source file containing functions exclusive to the SOA version */

#include "imagesoa.hpp"
#include "common_gauss.hpp"
#include "common_hst.hpp"
#include "common_mono.hpp"
#include <cstring>
#include <filesystem>
#include <iostream>

Image read_pixels(const std::filesystem::path &path, uint32_t start, uint32_t width, uint32_t height)
// Reads the RGB values of each pixel in the image
{
    std::ifstream f;
    f.open(path, std::ios::in | std::ios::binary);
    /* We do not need to check if it exists or if it could be opened because
   * read_header() already did */

    f.seekg(start);// Go to the address where the image data starts

    int px = int(width * height);
    std::vector<uint8_t> blue(px);
    std::vector<uint8_t> green(px);
    std::vector<uint8_t> red(px);

    /* To calculate how many bytes of padding we have in each row, first we find
   * out how many bytes the colors take up: that is three bytes per color. That
   * amount modulo 4 tells us how many bytes over we are, and 4 minus that
   * amount gives us how many bytes we have left. If the color bytes modulo 4 is
   * 0, then that amount is 4, and we may skip information from the image, so we
   * apply modulo 4 once again to convert that to 0.
   */
    const int padding_bytes = (4 - (int(width) * 3) % 4) % 4;
    int read = 0;

    for (int i = 0; i < px; i++) {
        f.read(reinterpret_cast<char *>(&blue[i]), sizeof(uint8_t));
        read++;
        f.read(reinterpret_cast<char *>(&green[i]), sizeof(uint8_t));
        read++;
        f.read(reinterpret_cast<char *>(&red[i]), sizeof(uint8_t));
        read++;
        if (read == static_cast<int>(width) * 3) {
            read = 0;
            f.ignore(padding_bytes);
        }
    }

    Image img{red, green, blue};
    return img;
}

void write_bmp(std::filesystem::path &path, const Header &header, Image image)
// Writes a (valid) bitmap file in the specified directory using a given header
// and the color values for its pixels
{
    write_header(path, header);

    std::ofstream f;
    f.open(path, std::ios::in | std::ios::binary);
    /* We do not need to check for errors because write_header() already did */

    f.seekp(static_cast<int>(header.img_start));

    const int padding_bytes =
            (4 - (static_cast<int>(header.img_width) * 3) % 4) % 4;
    int px = int(header.img_width * header.img_height);
    int zero = 0;
    int wrote = 0;
    for (int i = 0; i < px; i++) {
        f.write(reinterpret_cast<char *>(&image.b[i]), sizeof(uint8_t));
        wrote++;
        f.write(reinterpret_cast<char *>(&image.g[i]), sizeof(uint8_t));
        wrote++;
        f.write(reinterpret_cast<char *>(&image.r[i]), sizeof(uint8_t));
        wrote++;
        if (wrote == static_cast<int>(header.img_width) * 3) {
            f.write(reinterpret_cast<const char *>(&zero), padding_bytes);
            wrote = 0;
        }
    }
}

void frequencies(const std::vector<uint8_t> &color, std::ofstream &f) {
    std::vector<int> freq;// Declare a vector to count the occurrences of each value (from 0 to 255) of the color in each pixel
    freq.resize(256);
    for (uint8_t element: color) {
        // Every time a value appears, we add 1 to the element in the vector of frequencies that represents that value
        freq[element]++;
    }
    print_to_file(freq, f);
}

void histogram(const Image &img, std::filesystem::path path) {
    std::ofstream f = open_file(path);

    frequencies(img.r, f);
    frequencies(img.g, f);
    frequencies(img.b, f);
}

std::vector<int> getmim(int i, int j, const Header &h, const std::vector<uint8_t> &color) {
    std::vector<int> im;
    im.resize(25);
    int c = 0;
    for (int s = -3; s < 2; s++) {
        for (int t = -3; t < 2; t++) {
            int iteration = ((i + s) * static_cast<int>(h.img_width)) + (j + t);
            if (i + s < 0 || j + t < 0 || i + s > static_cast<int>(h.img_height) - 1 || j + t > static_cast<int>(h.img_width) - 1) {
                im[c] = 0;
            } else {
                int integer = static_cast<int>(color[iteration]);
                im[c] = getm(s, t) * integer;
            }
            c++;
        }
    }
    return im;
}

int getres(int i, int j, const Header &h, const std::vector<uint8_t> &color) {
    int w = 273;
    std::vector<int> im = getmim(i, j, h, color);
    int res = 0;
    for (int k = 0; k < 25; k++) {
        res += im[k];
    }
    return res / w;
}

Image gauss(const Image &img, const Header &h) {
    Image res;
    res.r.resize(img.r.size());
    res.g.resize(img.g.size());
    res.b.resize(img.b.size());

    int rr, rg, rb;
    for (int i = 0; i < static_cast<int>(h.img_height); i++) {
        for (int j = 0; j < static_cast<int>(h.img_width); j++) {
            int iteration = (i * static_cast<int>(h.img_width)) + j;
            rr = getres(i, j, h, img.r);
            res.r[iteration] = static_cast<uint8_t>(rr);
            rg = getres(i, j, h, img.g);
            res.g[iteration] = static_cast<uint8_t>(rg);
            rb = getres(i, j, h, img.b);
            res.b[iteration] = static_cast<uint8_t>(rb);
        }
    }
    return res;
}

void call_histogram(const Image &image, const std::filesystem::path &new_file) {
    std::filesystem::path output_file = new_file.parent_path();
    output_file /= new_file.stem();
    output_file += ".hst";
    histogram(image, output_file);
}

Image mono(Image &image)
// Calls all phases of the grayscale algorithm in order
{
    Image grayscale_img;
    for (int i = 0; i < static_cast<int>(image.r.size()); i++) {
        std::vector<uint8_t> colors = {image.r[i], image.g[i], image.b[i]};
        std::vector<double> norm_col = normalize(colors);
        norm_col = linear_intensity_transform(norm_col);
        double gray = gamma_correct(linear_transform(norm_col[0], norm_col[1], norm_col[2]));
        u_int8_t den_gray = denormalize(gray);
        image.r[i] = den_gray;
        image.g[i] = den_gray;
        image.b[i] = den_gray;
    }
    return grayscale_img;
}

Image perform_op(Image image, std::string &op, const std::filesystem::path &new_file, const Header &header) {
    const char *string = op.c_str();
    if (strcmp(string, "histo") == 0) {
        call_histogram(image, new_file);
    }
    if (strcmp(string, "mono") == 0) {
        mono(image);
    }
    if (strcmp(string, "gauss") == 0) {
        image = gauss(image, header);
    }
    return image;
}
