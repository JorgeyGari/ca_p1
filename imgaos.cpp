//
// Created by laura on 23/10/2022.
//

#include <iostream>
#include<cstring>
#include<filesystem>
#include "aos.cpp"
#include "progargs.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    Datastruct data;
    argparsing(argc, argv, &data);

    filesystem::path in_path(argv[1]);
    Header h = read_header("../in/elephant.bmp");
    filesystem::path path = "../in/elephant.bmp";
    std::vector<Pixel> pixels = read_pixels(path, h.img_start, h.img_width, h.img_height);
    histogram(pixels);
}