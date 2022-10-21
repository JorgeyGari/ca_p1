#include <iostream>
#include<cstring>
#include<filesystem>
#include "aos.cpp"


using namespace std;


int main(int argc, char *argv[]) {
    char *in_file = argv[1];
    char *out_file = argv[2];
    char *op = argv[3];
    if (argc != 4) {    /* Checks if enough arguments were provided */
        printf("\nWrong format: \n  image  in_path  out_path  oper \n    operation: copy, histo, mono, gauss \n");
        return -1;
    } else if ((strcmp(op, "copy") != 0) && (strcmp(op, "histo") != 0) && (strcmp(op, "mono") != 0) &&
               (strcmp(op, "gauss") != 0)) {
        printf("Unexpected operation: %s \n  image  in_path  out_path  oper \n   operation: copy, histo, mono, gauss \n",
               op);
        return -1;
    } else {
        printf("\nInput path: %s \nOutput path: %s \n", in_file, out_file);
        if (not(filesystem::exists(in_file))) {
            printf("Cannot open directory [%s]\n  image in_path out_path oper\n    opertion: copy, histo, mono, gauss",
                   in_file);
            return -1;
        } else if (not(filesystem::exists(out_file))) {
            printf("Output directory [%s] does not exist\n  image in_path out_path oper\n    opertion: copy, histo, mono, gauss",
                   out_file);
            return -1;
        } else {
            //AQUÍ IRIA EL CÓDIGO PRINCIPAL
            filesystem::path in_path(in_file);
            read_bmp(in_path);  //call function to read images
            filesystem::path out_path(out_file);
            //execute the operations and store in output file

            return 0;
        }
    }
}
