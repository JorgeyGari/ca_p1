#include "../imageaos.cpp"
#include <filesystem>
#include <gtest/gtest.h>
#include <string>

TEST(unit_test, test_read_pixels_aos)
{
    std::cout << "\nTEST: read_pixels() (AOS ver.)\n";
    auto x = read_pixels(std::filesystem::path("../../unit_test/test_in/test.bmp"), 138, 5, 5);

    Pixel red = {255, 0, 0};
    Pixel black = {0, 0, 0};
    Pixel white = {255, 255, 255};

    std::cout << "Checking pixel 0 is red.\n";
    EXPECT_EQ(red.r, x[0].r);
    EXPECT_EQ(red.g, x[0].g);
    EXPECT_EQ(red.b, x[0].b);

    std::cout << "Checking pixel 5 is white.\n";
    EXPECT_EQ(white.r, x[5].r);
    EXPECT_EQ(white.g, x[5].g);
    EXPECT_EQ(white.b, x[5].b);

    std::cout << "Checking pixel 15 is black.\n";
    EXPECT_EQ(black.r, x[15].r);
    EXPECT_EQ(black.g, x[15].g);
    EXPECT_EQ(black.b, x[15].b);
}

TEST(unit_test, test_mono_aos)
{
    std::cout << "\nTEST: mono() (AOS ver.)\n";
    auto x = read_pixels(std::filesystem::path("../../unit_test/test_in/test.bmp"), 138, 5, 5);
    x = mono(x);

    std::cout << "Checking every pixel is grayscale (R = G = B).\n";
    for(auto & i : x)
    {
        EXPECT_EQ(i.r, i.g);
        EXPECT_EQ(i.r, i.b);
    }
}

TEST(unit_test, test_histogram_aos)
{
    std::cout << "\nTEST: histogram() (AOS ver.)\n";
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
