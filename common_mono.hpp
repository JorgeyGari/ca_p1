/* Header containing definitions common to the AOS and SOA mono methods */

#ifndef IMAGE_COMMON_MONO_HPP
#define IMAGE_COMMON_MONO_HPP

#include <vector>

std::vector<double> normalize(std::vector<u_int8_t> colors);

std::vector<double> linear_intensity_transform(std::vector<double>normalized_colors);

double linear_transform(double red, double green, double blue);

double gamma_correct(double gray);

u_int8_t denormalize(double gray);

#endif//IMAGE_COMMON_MONO_HPP
