#include <iostream>
#include "soa.cpp"

int main() {
    std::filesystem::path p("../in/elepant.bmp");
    read_bmp(p);
}
