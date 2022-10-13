#include <iostream>
#include "soa.cpp"

int main() {
    std::filesystem::path p("../in/elephant.bmp");
    read_bmp(p);
}
