#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(int argc, char**argv) {
	FILE *fin, *fout;
	unsigned char *input=NULL, *output=NULL;;
	int pels=1920, lines=1035;
	int sizeIn=pels*lines*2; 	// size of input frame
	int sizeOut=1920*1024*2;	// size of output frame, crop to 1024 rows/lines
	//char inputname[80];
	char *inputname="outCpp.yuv";	// output from YCbCr->UYVY
	char *outputname="outChop.yuv";	
	int status;
	int i;

	input	= new unsigned char [sizeIn];
	output	= new unsigned char [sizeOut];

	// read in Y	
	inputname="outCpp.yuv";
	if ((fin=fopen(inputname,"rb")) == NULL){
		printf("Unable to open file %s!\n", inputname);
		exit(-1);
	}
	rewind(fin);
	if ((status = fread(input, 1, sizeIn, fin)) != sizeIn){
		fclose(fin);
		return 0;
	}
	fclose(fin);
	fin=NULL;

	memcpy(output, input, sizeOut);

	// write output file
	if ((fout = fopen(outputname, "wb")) == NULL){
		printf("Error in opening file %s!\n", outputname);
		exit(-1);
	}
	if ((status = fwrite(output, 1, sizeOut, fout)) != sizeOut){
		printf("Error in writing to file %s!\n", outputname);
		exit(-1);
	}	
	fclose(fout);
	fout=NULL;

	return 1;
}


