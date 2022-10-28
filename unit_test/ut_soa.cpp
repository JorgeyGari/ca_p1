#include "../imgsoa.cpp"
#include <filesystem>
#include <gtest/gtest.h>
#include <string>

TEST(unit_test, test_read_pixels_soa)
{
    std::cout << "\nTEST: read_pixels() (SOA ver.)\n";
    auto x = read_pixels(std::filesystem::path("../../unit_test/test_in/test.bmp"), 138, 5, 5);

    std::vector<uint8_t> red_RGB = {255, 0, 0};
    std::vector<uint8_t> black_RGB = {0, 0, 0};
    std::vector<uint8_t> white_RGB = {255, 255, 255};

    std::cout << "Checking pixel 0 is red.\n";
    EXPECT_EQ(red_RGB[0], x.r[0]);
    EXPECT_EQ(red_RGB[1], x.g[0]);
    EXPECT_EQ(red_RGB[2], x.b[0]);

    std::cout << "Checking pixel 5 is white.\n";
    EXPECT_EQ(white_RGB[0], x.r[5]);
    EXPECT_EQ(white_RGB[1], x.g[5]);
    EXPECT_EQ(white_RGB[2], x.b[5]);

    std::cout << "Checking pixel 15 is black.\n";
    EXPECT_EQ(black_RGB[0], x.r[15]);
    EXPECT_EQ(black_RGB[1], x.g[15]);
    EXPECT_EQ(black_RGB[2], x.b[15]);
}

TEST(unit_test, test_mono_soa)
{
    std::cout << "\nTEST: mono() (SOA ver.)\n";
    auto x = read_pixels(std::filesystem::path("../../unit_test/test_in/test.bmp"), 138, 5, 5);
    x = mono(x);

    std::cout << "Checking every pixel is grayscale (R = G = B).\n";
    EXPECT_EQ(x.r, x.g);
    EXPECT_EQ(x.r, x.b);
}

TEST(unit_test, test_histogram_soa)
{
    std::cout << "\nTEST: histogram() (SOA ver.)\n";
    auto x = read_pixels(std::filesystem::path("../../unit_test/test_in/test.bmp"), 138, 5, 5);
    histogram(x, std::filesystem::path("../../unit_test/test_out/test.hst"));
    std::ifstream f = std::filesystem::path("../../unit_test/test_out/test.hst");

    std::string s, pixels_with_red_0 = "5", pixels_with_red_255 = "20", pixels_with_green_255 = "5";
    std::getline(f, s);
    std::cout << "Checking the number of pixels with a red value of 0 is the expected (black pixels).\n";
    EXPECT_EQ(s, pixels_with_red_0);

    for (int i = 0; i < 255; i++)
    {
        std::getline(f, s);
    }
    std::cout << "Checking the number of pixels with a red value of 255 is the expected (red pixels + white pixels).\n";
    EXPECT_EQ(s, pixels_with_red_255);

    for (int i = 0; i < 256; i++)
    {
        std::getline(f, s);
    }
    std::cout << "Checking the number of pixels with a green value of 255 is the expected (white pixels).\n";
    EXPECT_EQ(s, pixels_with_green_255);
}
