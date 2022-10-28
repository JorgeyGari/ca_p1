/* Source file containing common functions for SOA and AOS mono methods */

#include "common_mono.hpp"
#include <cmath>
#include <vector>

std::vector<double> normalize(std::vector<u_int8_t> colors)
// Normalize (divide by 255) the RGB values of a pixel
{
    std::vector<double> normalized_colors;

    normalized_colors.push_back(static_cast<double>(colors[0]) / 255);
    normalized_colors.push_back(static_cast<double>(colors[1]) / 255);
    normalized_colors.push_back(static_cast<double>(colors[2]) / 255);

    return normalized_colors;
}

std::vector<double> linear_intensity_transform(std::vector<double> normalized_colors)
// Applies a transformation to the normalized colors' intensity
{
    std::vector<double> new_color;
    for (int i = 0; i < 3; i++) {
        if (normalized_colors[i] <= 0.0405) {
            new_color.push_back(normalized_colors[i] / 12.92);
        } else {
            new_color.push_back(std::pow((normalized_colors[i] + 0.055) / 1.055, 2.4));
        }
    }
    return new_color;
}

double linear_transform(double red, double green, double blue)
// Applies a linear transformation to the intensity-normalized colors and obtains the gray value
{
    return 0.2126 * red + 0.7152 * green + 0.0722 * blue;
}

double gamma_correct(double gray)
// Applies a gamma correction to the gray value obtained
{
    if (gray <= 0.0031308) {
        return 12.92 * gray;
    }

    return 1.055 * pow(gray, 1 / 2.4) - 0.055;
}

u_int8_t denormalize(double gray)
// Denormalizes the gray value by multiplying it by 255
{
    return static_cast<u_int8_t>(gray * 255);
}
