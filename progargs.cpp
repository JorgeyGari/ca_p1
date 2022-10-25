#include <iostream>
#include<cstring>
#include<filesystem>
#include<string>

using namespace std;

struct Datastruct{
	filesystem::path in, out;
};


struct Datastruct argparsing(string in, string out, string oper){
	
	Datastruct data;
	const char *in_file= in.c_str();
	const char *out_file=out.c_str();
	const char *op=oper.c_str();
   
     if ((strcmp(op, "copy") != 0) && (strcmp(op, "histo") != 0) && (strcmp(op, "mono") != 0) && (strcmp(op, "gauss") != 0)) {
        printf("Unexpected operation: %s \n  image  in_path  out_path  oper \n   operation: copy, histo, mono, gauss \n",
               op);
        exit(-1);
    } else {
        printf("\nInput path: %s \nOutput path: %s \n", in_file, out_file);
        if (not(filesystem::exists(in_file))) {
            printf("Cannot open directory [%s]\n  image in_path out_path oper\n    opertion: copy, histo, mono, gauss \n",
                   in_file);
            exit(-1);
        } else if (not(filesystem::exists(out_file))) {
         	 printf("Output directory [%s] does not exist\n  image in_path out_path oper\n    opertion: copy, histo, mono, gauss \n",
                   out_file);
            	exit(-1);
        } else {
            filesystem::path in_path(in_file);
            data.in= in_path;
            filesystem::path out_path(out_file);
            data.out=out_path;
	    return data;

        }
    }
}

