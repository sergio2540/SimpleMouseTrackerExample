#include "cv.h"
#include "highgui.h"
#include <stdio.h>


//detectlaser
//main x y side thresh greater
//main 20 20 5 255 false

int main(int argc,char*argv[]){
	
        //Extract the color information
	IplImage * mouse = cvLoadImage("rato.jpg",CV_LOAD_IMAGE_COLOR);

	int width = mouse->width;
	int height = mouse->height;
	int depth = mouse->depth;
	int nChannels = mouse->nChannels;

	IplImage *hsv = cvCreateImage(cvSize(width,height),depth,nChannels);
	
	IplImage *hue = cvCreateImage(cvSize(width , height), depth, 1);
	IplImage *sat = cvCreateImage(cvSize(width , height), depth, 1);
	IplImage *img = cvCreateImage(cvSize(width , height), depth, 1);

	cvCvtColor(img, hsv, CV_RGB2HSV);
	cvSplit(hsv,hue,sat,img,(CvArr*) NULL);
	

	
	/*
	int x = atoi(argv[1]);
	int y = atoi(argv[2]);
	int side = atoi(argv[3]);
	int thresh = atoi(argv[4]);
	
	int greater = atoi(argv[5]);
	*/

	int x = 20;
	int y = 20;
	int side = 5;
	int thresh = 255;
	int greater = 0;

	//int i = y-side;
	int i = 15;
	//int iend = y+side;
	int iend = 25;

	//int j = x-side;
	int j = 15;
	//int jend = x+side;
	int jend = 25;

	int sum = 0;

	for(;i < iend;i++){
		for(;j < jend;j++){
			sum += cvGet2D(img,i,j).val[0];
			//printf("Sum: %d",sum);		
		}	
	}

	printf("Sum: %d",sum);

	float mean = sum/side;
	float ret[2];
	ret[0] = (greater && mean > thresh) || (!greater && mean < thresh);
	ret[1] = mean;

	printf("Greater: %f\n",ret[0]);

	printf("Mean: %f\n",ret[1]);

	return 0;
       
}
