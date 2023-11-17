#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// UYVU = U Y V Y U Y V Y U Y V Y U Y V Y
int main(int argc, char**argv) {
	FILE *fin, *fout;
	unsigned char *input=NULL, *output=NULL, *outputBff=NULL; // Bff=bottom field first
	unsigned char *justYUV=NULL, *justY=NULL, *justU=NULL, *justV=NULL;
	unsigned char *justY2=NULL, *justU2=NULL, *justV2=NULL;
	unsigned char *justYUVnew=NULL, *uyuv=NULL;
	int pels=1920, lines=1035;
	int sizeIn=pels*lines*2; 	// size of input frame
	int sizeOut=1920*1024*2;	// size of output frame, crop to 1024 rows/lines
	//char inputname[80];
	char *inputname="outUYVY.yuv";	// output from YCbCr->UYVY
	char *outputname="outChop.yuv";	
	int status;
	int frame;
	
	int startFrame=200;
	int noFrames=600;
	//int startFrame=0;
	//int noFrames=1;

	int i, y_inc, u_inc, v_inc;
	int marker;
	int y_inc2, u_inc2, v_inc2;

	//system("del outChop.yuv");

	input	= new unsigned char [sizeIn];
	output	= new unsigned char [sizeOut];
	outputBff = new unsigned char [sizeOut];
	justYUV = new unsigned char [sizeOut];
	justY=new unsigned char [1920*1024];
	justU=new unsigned char [1920*512];
	justV=new unsigned char [1920*512];
	justY2=new unsigned char [1920*1024];
	justU2=new unsigned char [1920*512];
	justV2=new unsigned char [1920*512];
	justYUVnew = new unsigned char [sizeOut];
	uyuv = new unsigned char [sizeOut];

	// open input file
	if ((fin=fopen(inputname,"rb")) == NULL){
		printf("Unable to open file %s!\n", inputname);
		exit(-1);
	}
	rewind(fin);

	// open output file
	if ((fout = fopen(outputname, "ab")) == NULL){
		printf("Error in opening file %s!\n", outputname);
		exit(-1);
	}

	for (frame=0; frame < noFrames; frame++){
		printf("frame=%d\n",frame);
		// read a full UYVY frame
		if ((status = fread(input, 1, sizeIn, fin)) != sizeIn){
			fclose(fin);
			return 0;
		}

		// start from frame "startFrame", ie: chop off first (0->startFrame-1) frames
		if (frame >= startFrame){	
			// copy 1st 1024 rows/lines ... cropping
			memcpy(output, input, sizeOut);

			// "output" is the frame 1920x1024, U Y V Y U Y V Y
			y_inc=0;
			u_inc=0;
			v_inc=0;
			for (i=1; i < (1920*1024*2); i=i+2){
			       justY[y_inc++]=output[i];	
			}
			for (i=0; i < (1920*1024*2); i=i+4){
			       justU[u_inc++]=output[i];	
			       justV[v_inc++]=output[i+2];
			}
		/*
			// create YUV 422 block, Y 1920x1024, U 1920x512 and V 1920x512
			for (i=0; i<(1920*1024); i++){
				justYUV[i]=justY[i];
			}
			u_inc=0;
			for (i=(1920*1024); i < (1920*1024)+(1920*512); i++){
			justYUV[i] = justU[u_inc++];
			}
			v_inc=0;
			for(i=(1920*1024)+(1920*512); i < (1920*1024*2); i++){
				justYUV[i] = justV[v_inc++];
			}
			// convert YUV422 block to bottom field first (justYUVnew)
			// Y
			for (i=0; i<1964160; i++){
				justYUVnew[i] = justYUV[i+1920];
			}
			for(i=1962240; i<=1964159; i++){
				justYUVnew[i+1920]=justYUVnew[i];
			}
	
			// just copy the chroma
			//for(i=1966080;i<=3932159;i++){
			//	justYUVnew[i]=justYUV[i];
			//}
		
			// U
			for(i=1966080; i<2947200; i++){
				justYUVnew[i]=justYUV[i+1920];
			}
			for(i=2945280;i<=2947199; i++){
				justYUVnew[i+1920]=justYUVnew[i];
			}
			// V
			for (i=2949120; i<3930240; i++){
				justYUVnew[i]=justYUV[i+1920];
			}
			for(i=3928320;i<=3930239;i++){
				justYUVnew[i+1920]=justYUVnew[i];
			}
			
			// get just the Y U and V from bottom field first YUV422 block
			y_inc2=0;
			for (i=0; i<1966080; i++){
				justY2[y_inc2++]=justYUVnew[i];
			}
			u_inc2=0;
			for (i=1966080; i<2949120; i++){
				justU2[u_inc2++]=justYUVnew[i];
			}
			v_inc2=0;
			for(i=2949120;i<3932160; i++){
				justV2[v_inc2++]=justYUVnew[i];
			}
	*/

			// bottom field first 
			for (i=0; i < 1964160; i++){
				justY2[i]=justY[i+1920];
			}
			for (i=1964160; i<=1966079;i++) {
				justY2[i]=justY[i];
			}
			for (i=0; i<981120; i++){
				justU2[i]=justU[i+1920];
			}
			for (i=981120; i<=983039;i++){
				justU2[i]=justU[i];
			}
			for (i=0;i<981120;i++){
				justV2[i]=justV[i+1920];
			}
			for(i=981120;i<=983039;i++){
				justV2[i]=justV[i];
			}


			marker=0;	
			y_inc2=0;
			u_inc2=0;
			v_inc2=0;				
			// rearrange to UYUV	
			for (i=0; i<(1920*1024*2);i++){
				switch (marker) {
					case 0:
						// U
						output[i] = justU2[u_inc2++]; 
						marker=1;
						break;
					case 1:
						// Y
						output[i] = justY2[y_inc2++];
						marker=2;
						break;
					case 2: 
						// V
						output[i] = justV2[v_inc2++];
						marker=3;
						break;
					case 3:
						// Y
						output[i] = justY2[y_inc2++];
						marker=0;
						break;
					default: 
						printf("Switch marker error!\n");
						break;
				}
			}
	
			// Now run Munich subsampling on the bottom field first UYUV to convert to i420
			

			// write cropped frame to output
			if ((status = fwrite(output, 1, sizeOut, fout)) != sizeOut){
				printf("Error in writing to file %s!\n", outputname);
				exit(-1);
			}	
		}
	}
	
	fclose(fin);
	fin=NULL;
	fclose(fout);
	fout=NULL;

	return 1;
}


