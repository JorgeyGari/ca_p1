#include <iostream>
#include<cstring>	
#include<filesystem>

using namespace std;


int main(int argc,char *argv[]){
	char *in_file= argv[1];
	char *out_file=argv[2];
	char *op=argv[3];
	if (argc != 4){    /* Checks if enough arguments were provided */    
   		 printf("\nWrong format: \n  image  in_path  out_path  oper \n    operation: copy, histo, mono, gauss \n");
   		 return -1;
 	 }else if((strcmp(op,"copy")!=0)&&(strcmp(op,"histo")!=0)&&(strcmp(op,"mono")!=0)&&(strcmp(op,"gauss")!=0)){
		printf("Unexpected operation: %s \n  image  in_path  out_path  oper \n   operation: copy, histo, mono, gauss \n", op);
		return -1;
	}else{
	printf("\nInput path: %s \nOutput path: %s \n", in_file, out_file);
		if (not(filesystem::exists(in_file))){ //la ouputfile donde se imprimem los resultados tiene que existir o la creamos nosotros?
			printf("Cannot open directory [%s]\n", in_file);
			return -1;
		}else {  
		//AQUÍ IRIA EL CÓDIGO PRINCIPAL 
	
		}
		return 0;
	}
}
