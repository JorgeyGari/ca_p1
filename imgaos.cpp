/* Main source file for the AOS version */

#include "aos.cpp"
#include "progargs.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    Datastruct data;
    argparsing(argc, argv, &data);

    Header header = read_header(data.in);
    std::vector<Pixel> image = read_pixels(data.in, header.img_start, header.img_width, header.img_width);
    write_bmp(data.out, header, image);
}
