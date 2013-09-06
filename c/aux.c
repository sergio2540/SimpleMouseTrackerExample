#include "cv.h"
#include "highgui.h"
#include <stdio.h>


// args: img, mask, mask_not, thresh1, thresh2
int main(int argc, char* argv[]){


  //Extract the color information
  IplImage *mouse = cvLoadImage("mouse.jpg",CV_LOAD_IMAGE_COLOR);


  //Load masks 
  IplImage *mask = cvLoadImage("mask.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  IplImage *mask_not = cvLoadImage("mask_not.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  //const CvArr* img = argv[1];
  //const CvArr* mask = argv[2];

  int width = mouse->width;
  int height = mouse->height;
  int depth = mouse->depth;
  int nChannels = mouse->nChannels;
  printf("width: %d\n", width);
  printf("height: %d\n", height);
  IplImage *hsv = cvCreateImage(cvSize(width,height),depth,3);
  printf("channels hsv: %d\n",mask->nChannels);
  IplImage *hue = cvCreateImage(cvSize(width , height), depth, 1);
  IplImage *sat = cvCreateImage(cvSize(width , height), depth, 1);
  IplImage *img = cvCreateImage(cvSize(width , height), depth, 1);

  cvCvtColor(mouse, hsv, CV_RGB2HSV);
  cvSplit(hsv,hue,sat,img,(CvArr*) NULL);

  //
  //  get a rough region around the mouse using the first threshold
  //
  double thresh1 = 70; // argv[3];
  IplImage *threshold = cvCreateImage(cvSize(img->width,img->height),img->depth,1);
  double max_value = 255;
  cvThreshold(img, threshold, thresh1, max_value, CV_THRESH_BINARY_INV);
  
  IplImage *bitwise_and = cvCreateImage(cvSize(width , height), depth, 1);

  // get only the arena
  cvAnd(threshold , mask, bitwise_and, NULL);

  printf("bitwise_and: width: %d\n", bitwise_and->width);
  printf("bitwise_and: height: %d\n", bitwise_and->height);
  // clean up noise
  int cols = 3;
  int rows = 3;
  int anchor_x = 1;
  int anchor_y = 1;
  int shape = CV_SHAPE_RECT;
  IplConvKernel* kernel = cvCreateStructuringElementEx(cols, rows, anchor_x, anchor_y, shape, NULL );
  
  IplImage *erode = cvCreateImage(cvSize(width , height), depth, 1);
  int iterations = 2;
  cvErode(bitwise_and, erode, kernel, iterations);
  // Enlarge the interest region
  cols = 4;
  rows = 4;
  kernel = cvCreateStructuringElementEx(cols, rows, anchor_x, anchor_y, shape, NULL );
  IplImage *dilate = cvCreateImage(cvSize(width , height), depth, 1);
  cvDilate(erode, dilate, kernel, 10);
  
  // get the blobs
  CvMemStorage* storage = cvCreateMemStorage(0);
  CvSeq* contour = 0;
  int header_size = sizeof(CvContour);
  int mode = CV_RETR_LIST;
  int method = CV_CHAIN_APPROX_SIMPLE;
  CvPoint offset = cvPoint(0,0);
  int nContours = cvFindContours(dilate, storage, &contour, header_size, CV_RETR_LIST, method, offset );
  printf("Number of contours: %d\n", nContours);


  IplImage *contourImg = cvCreateImage(cvSize(width , height), depth, 3);

  for ( ; contour != 0; contour = contour->h_next ) {
    CvScalar externalColor = CV_RGB( 255, 255, 255 );
    CvScalar internalColor = CV_RGB( 0, rand()&255, 0 );
    cvDrawContours(contourImg, contour, externalColor, internalColor, -1, 2, 8 , offset);
    printf("Countour->total: %d\n", contour->total);
    printf("Countour->elem_size: %d\n", contour->elem_size);

  }
  // save image
  cvSaveImage("threshold.jpg", contourImg);
}
