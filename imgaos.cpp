/* Main source file for the AOS version */

#include "aos.cpp"
#include "progargs.cpp"
#include<chrono>

using namespace std;

void perform_op(Image *image){
	if (strcmp(argv[3], "copy") == 0) {
           
        } else if (strcmp(argv[3], "histo") == 0) {
            histogram(image);
            
        } else if (strcmp(argv[3], "mono") == 0) {
            //mono(image);
            
        } else {
            //gauss(image);
            	}
}

int main(argc, char *argv[]) {
	
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
        std::vector<Pixel> image = read_pixels(entry.path(), header.img_start, header.img_width, header.img_height);
	auto stop= chrono::high_resolution_clock::now();
	auto duration= chrono::duration_cast<std::chrono::microseconds>(stop - start);
	int loadtime= duration.count();

	auto start = chrono::high_resolution_clock::now();
        filesystem::path new_file = data_files.out;
        new_file /= entry.path().filename();
        open_file(new_file);
    	perform_op(&image);
        auto stop= chrono::high_resolution_clock::now();
	    auto duration= chrono::duration_cast<std::chrono::microseconds>(stop - start);
	    int opertime= duration.count();
        auto start = chrono::high_resolution_clock::now();
        write_bmp(new_file, header, image);
        auto stop= chrono::high_resolution_clock::now();
	    auto duration= chrono::duration_cast<std::chrono::microseconds>(stop - start);
	    int opertime= duration.count();

    int total=loadtime + opertime + storetime;     
	cout << "File: " << entry << " (time: " << total << ")" << "\n";//print the total time and the particular times
    cout<< "Load time: " << loadtime << "\n";
    cout << arg[3] << " time: " << opertime << "\n";
    cout<< "Store time: " << storetime << "\n";
    }
}
