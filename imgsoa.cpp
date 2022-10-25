/* Main source file for the SOA version */

#include "progargs.cpp"
#include "soa.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 4) { /* Checks if enough arguments were provided */
        printf("\nWrong format: \n  image  in_path  out_path  oper \n    operation: copy, histo, mono, gauss \n");
        exit(-1);
    }
	Datastruct data_files =argparsing(string(argv[1]),string(argv[2]),string(argv[3])); //analyzes if data provided is valid
											    //
//we want to iterate over the elements of the directory 
    filesystem::directory_iterator it(data_files.in);
    for (auto& entry : it){
            cout <<"File: "<<  entry << "\n"; //TAMBIÉN HAY QUE IMPRIMIR EL TIME DE CADA COSA
    Header header = read_header(data_files.in);
    Image image = read_pixels(data_files.in, header.img_start, header.img_width, header.img_height);
    write_bmp(data_files.out, header, image);
	}
}
