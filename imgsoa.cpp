/* Main source file for the SOA version */

#include "common.hpp"
#include "common_hst.hpp"
#include "imagesoa.cpp"
#include "progargs.cpp"
#include <chrono>

using namespace std;

int main(int argc, char *argv[]) {
    argvalidation(argc);
    Datastruct data_files = argparsing(string(argv[1]), string(argv[2]), string(argv[3]));//analyzes if data provided is valid
    filesystem::directory_iterator it(data_files.in);
    for (auto &entry: it) {
        auto start = chrono::high_resolution_clock::now();
        Header header = read_header(entry.path());
        Image image = read_pixels(entry.path(), header.img_start, header.img_width, header.img_height);
        long loadtime = stop_chrono(start);

        filesystem::path new_file = data_files.out;
        new_file /= entry.path().filename();

        start = chrono::high_resolution_clock::now();
        if(strcmp(argv[3], "histo") != 0) {
            open_file(new_file);
        }
        image = perform_op(image, reinterpret_cast<string &>(argv[3]), new_file, header);
        auto opertime = stop_chrono(start);

        start = chrono::high_resolution_clock::now();
        if (strcmp(argv[3], "histo") != 0) {
            write_bmp(new_file, header, image);
        }
        auto storetime = stop_chrono(start);

        long total = loadtime + opertime + storetime;
        std::cout << "----------------------\n";
        cout << "File: " << entry << " (time: " << total << ")" << "\n";//print the total time and the particular times
        print_data(string(argv[3]), loadtime, opertime, storetime);
    }
}
