/* Main source file for the AOS version */

#include "aos.cpp"
#include "progargs.cpp"

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
            cout <<"File: "<<  entry << "\n"; //print the total time and the particular times 
    Header header = read_header(data_files.in);
    std::vector<Pixel> image = read_pixels(data_files.in, header.img_start, header.img_width, header.img_height);
   	 if (strcmp(argv[3], "copy"){
   		 write_bmp(data_files.out, header, image);
   	 }else if (strcmp(argv[3], "histo"){
	 	histogram(image);
	      	write_bmp(data_files.out, header, image);	
	 }else if (strcmp(argv[3], "mono"){
		//mono(image);
		write_bmp(data_files.out, header, image);
	 }else{
	 	//gauss(image);
		write_bmp(data_files.out, header, image);
		 }
	}
}
