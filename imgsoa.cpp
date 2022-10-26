/* Main source file for the AOS version */

#include "soa.cpp"
#include "progargs.cpp"
#include<chrono>
#include<cstring>

using namespace std;

void perform_op(const Image &image, string &op, filesystem::path new_file, const Header &header) {
    const char *string = op.c_str();
    if (strcmp(string, "copy") == 0) {
        write_bmp(new_file, header, image);
    } else if (strcmp(string, "histo") == 0) {
        histogram(image);
        write_bmp(new_file, header, image);
    } else if (strcmp(string, "mono") == 0) {
        std::cout << "mono is not yet implemented, no modifications will be made to the images\n";
        write_bmp(new_file, header, image);
    } else {
        gauss(image, header);
        write_bmp(new_file, header, image);
    }
}

void print_data(long total, long loadtime, long opertime, long storetime){
    cout << "File: " << entry << " (time: " << total << ")" << "\n";//print the total time and the particular times
    cout<< "Load time: " << loadtime << "\n";
    cout << argv[3] << " time: " << opertime << "\n";
    cout<< "Store time: " << storetime << "\n";
}

auto stop_chrono(long start){
    chrono::time_point<chrono::system_clock, chrono::duration> stop= chrono::high_resolution_clock::now();
	    auto duration= chrono::duration_cast<std::chrono::microseconds>(stop - start);
	    auto opertime= duration.count();
        return opertime;
}

int main(int argc, char *argv[]) {
    if (argc != 4) { /* Checks if enough arguments were provided */
        printf("\nWrong format: \n  image  in_path  out_path  oper \n    operation: copy, histo, mono, gauss \n");
        exit(-1);
    }
    Datastruct data_files = argparsing(string(argv[1]), string(argv[2]), string(argv[3]));//analyzes if data provided is valid
                                                                                          //
                                                                                          //we want to iterate over the elements of the directory
    filesystem::directory_iterator it(data_files.in);
    for (auto &entry: it) {

        auto start = chrono::high_resolution_clock::now();
        Header header = read_header(entry.path());
        Image image = read_pixels(entry.path(), header.img_start, header.img_width, header.img_height);
        auto loadtime=stop_chrono(start);
    
    long start = chrono::high_resolution_clock::now();
    filesystem::path new_file = data_files.out;
        new_file /= entry.path().filename();
        open_file(new_file);

        perform_op(&image, string(argv[3]));
        auto opertime=stop_chrono(start);
        auto start = chrono::high_resolution_clock::now();
        write_bmp(new_file, header, image);
        auto storetime=stop_chrono(start);
        auto total= loadtime + opertime + storetime;
        print_data(total,loadtime,opertime,storetime);   
    }
}