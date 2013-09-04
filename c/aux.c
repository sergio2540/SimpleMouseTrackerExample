#include "cv.h"
#include "highgui.h"
#include <stdio.h>


// args: img, mask, mask_not, thresh1, thresh2
int main(int argc, char* argv[]){


  //Extract the color information
  IplImage * mouse = cvLoadImage("mouse.jpg",CV_LOAD_IMAGE_COLOR);
  //const CvArr* img = argv[1];
  int width = mouse->width;
  int height = mouse->height;
  int depth = IPL_DEPTH_8U;
  int nChannels = mouse->nChannels;
  
  printf("DEBUG: depth: %d", depth);

  IplImage *hsv = cvCreateImage(cvSize(width,height),depth,3);

  //printf("channels hsv: %d",hsv->nChannels);
  IplImage *hue = cvCreateImage(cvSize(width , height), depth, 1);
  IplImage *sat = cvCreateImage(cvSize(width , height), depth, 1);
  IplImage *img = cvCreateImage(cvSize(width , height), depth, 1);

  cvCvtColor(mouse, hsv, CV_RGB2HSV);
  cvSplit(hsv,hue,sat,img,(CvArr*) NULL);

  double thresh1 = 70; // argv[3];
  
  IplImage *dst = cvCreateImage(cvSize(img->width,img->height),img->depth,1);
  double max_value = 255;   

  cvThreshold(img, dst, thresh1, max_value, CV_THRESH_BINARY_INV);
  
  // save image
  cvSaveImage("threshold.jpg", dst);
}
