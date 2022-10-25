/* Main source file for the SOA version */

#include "soa.cpp"
#include "progargs.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    Datastruct data;
    argparsing(argc, argv, &data);

    Header header = read_header(data.in);
    Image image = read_pixels(data.in, header.img_start, header.img_width, header.img_height);
    histogram(image);
    Image res = gauss(image, header);
    write_bmp(data.out, header, image);
}