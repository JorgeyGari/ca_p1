/* Main source file for the AOS version */

#include "aos.cpp"
#include "progargs.cpp"

using namespace std;

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
        cout << "File: " << entry << "\n";//print the total time and the particular times
        Header header = read_header(entry.path());
        std::vector<Pixel> image = read_pixels(entry.path(), header.img_start, header.img_width, header.img_height);
        filesystem::path new_file = data_files.out;
        new_file /= entry.path().filename();
        open_file(new_file);
        if (strcmp(argv[3], "copy") == 0) {
            write_bmp(new_file, header, image);
        } else if (strcmp(argv[3], "histo") == 0) {
            std::filesystem::path output_file = data_files.out;
            output_file /= entry.path().stem();
            output_file += ".hst";
            histogram(image, output_file);
            write_bmp(new_file, header, image);
        } else if (strcmp(argv[3], "mono") == 0) {
            //mono(image);
            write_bmp(new_file, header, image);
        } else {
            std::vector<Pixel> res = gauss(image, header);
            write_bmp(new_file, header, res);
        }
    }
}
