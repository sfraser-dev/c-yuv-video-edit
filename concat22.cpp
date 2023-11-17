#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(int argc, char**argv) {
	FILE *fin, *fout;
	unsigned char *input=NULL, *output=NULL;
	int pels=720, lines=486;
	int size=pels*lines*3/2;
	char inputname[80];
	char outputname[80];
	char *prefix=NULL;
	int frame;
	int maxFrameNumber; // 0 upto this number minus 1 
	int status;
	int i;

	input	= new unsigned char [size];
	output	= new unsigned char [size];

	// process the arguments
	for (i = 1; i < argc; i++){
	        if (*(argv[i]) == '-'){
           	    	switch (*(++argv[i])){
                		case 'n':
                    			maxFrameNumber = atoi(argv[++i]);
                    			break;
				case 'o':
                    			strcpy(outputname, argv[++i]);
                    			break;
                		default:
                		    	printf("Illegal option %c!\n", *argv[i]);
					printf("Usage: concat2.exe -n <noOfFrames> -o <output planar YUV 420 file>\n");
    					printf("Concatenates <noOfFrames-1> frames of\n\n");
					printf("Motion_Graphics.000_planar420.yuv\n"); 
					printf("Motion_Graphics.001_planar420.yuv\n"); 
					printf("Motion_Graphics.002_planar420.yuv\n"); 
					printf(".\n");
					printf(".\n");
					printf(".\n");
					printf("Motion_Graphics.<noOfFrames-1>_planar420.yuv\n"); 
					printf("\ninto a single planar YUV 420 file.\n\n");
				    	return -1;
            	    	}
        	}
    	}

	if ((fout = fopen(outputname, "ab")) == NULL){
		printf("Error in opening file %s!\n", outputname);
		exit(-1);
	}

	for (frame = 0; frame < maxFrameNumber; frame++){
		if (frame < 10){
			prefix="00";
		}
		else if (frame < 100){
			prefix="0";
		}
		else {
			prefix="";
		}
		sprintf(inputname,"Motion_Graphics.%s%d.yuv",prefix,frame);
		//sprintf(inputname,"Motion_Graphics.000_planar420.yuv");
		printf("%d %s\n",frame,inputname);

		if (inputname == NULL || input == NULL){
			printf("Null pointer in call to read unsigned char!\n");
			exit(-1);
		}

		if ((fin=fopen(inputname,"rb")) == NULL){
			printf("Unable to open file %s!\n", inputname);
			exit(-1);
		}
		rewind(fin);

		if ((status = fread(input, 1, size, fin)) != size){
			fclose(fin);
			return 0;
		}
		fclose(fin);
		fin=NULL;
	
	    
		memcpy(output, input, size);
		
	
		if ((status = fwrite(output, 1, size, fout)) != size){
			printf("Error in writing to file %s!\n", outputname);
			exit(-1);
		}		
	}

	fclose(fout);	
	return 1;
}


