#include <stdio.h>
#include <stdlib.h>

void copy_content(char *in_file, char *out_file){
	int ch; // store char that move from input file to output file 
	FILE *inptr = fopen(in_file, "r");
	if(!inptr){
		printf("cannot open input file!\n");
		exit(0);
	}
	FILE *outptr = fopen(out_file, "w");
	if(!outptr){
		printf("cannot open output file!\n");
		exit(0);
	}
	while((ch = fgetc(inptr)) != EOF){ // read from input file and write to output file
		fputc(ch, outptr);
	}
	fclose(inptr);
	fclose(outptr);
	printf("content successfully copied to output file\n");
}

int main(){
	FILE *tmpptr; // use to check if file exists
	char in_file[100], out_file[100]; // file name

	// aquire input file name
	printf("please enter the name of input file : ");
	scanf("%s", in_file);
	// aquire output file name
	printf("please enter the name of output file : ");
	scanf("%s", out_file);
	// check if input file exists
	if( (tmpptr=fopen(in_file, "r")) == NULL){
		printf("input file does not exist!\n");
		exit(0);
	}
	// check if output file exists
	if( (tmpptr=fopen(out_file, "r")) != NULL){
		printf("output file exists!\n");
		exit(0);
	}			
	copy_content(in_file, out_file);
	return 0;	
}
