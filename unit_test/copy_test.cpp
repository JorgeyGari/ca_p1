#include <gtest/gtest.h>
#include <filesystem>
#include "../aos.cpp"

TEST(copy_test, test_identical)
{
    auto x = read_pixels(std::filesystem::path("../unit_test/test_in/test.bmp"), 138, 5, 5);
    Pixel red = {255, 0, 0};
    Pixel black = {0, 0, 0};
    Pixel white = {255, 255, 255};
    EXPECT_EQ(red.r, x[0].r);
    EXPECT_EQ(red.g, x[0].g);
    EXPECT_EQ(red.b, x[0].g);
    EXPECT_EQ(white.r, x[5].r);
    EXPECT_EQ(white.g, x[5].g);
    EXPECT_EQ(white.b, x[5].b);
    EXPECT_EQ(black.r, x[15].r);
    EXPECT_EQ(black.g, x[15].g);
    EXPECT_EQ(black.b, x[15].b);
}
