#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>

float norm(int Cx, int Cy, int px, int py){

  float val = sqrt((Cx - px)*(Cx - px) + (Cy - py)*(Cy - py));

  return val;
}

CvPoint* getFurthestPoint(CvSeq *contour, int Cx, int Cy){

  CvPoint *r=NULL, *n=NULL;

  int j;
  
  float maxNorm = 0.0;
  float currNorm = 0.0;


  for(j = 0; j < contour->total; j++ ){

      r = CV_GET_SEQ_ELEM( CvPoint, contour, j );

      currNorm = norm(Cx, Cy, r->x, r->y);

      if (currNorm > maxNorm) {
        n = r;
        maxNorm = currNorm;
      }

    }

    return n;
}



// args: img, mask, mask_not, thresh1, thresh2
int main(int argc, char* argv[]){

  //Extract the color information
  IplImage *mouse = cvLoadImage("mouse.jpg",CV_LOAD_IMAGE_COLOR);

  //get mouse image info
  int width = mouse->width;
  int height = mouse->height;
  int depth = mouse->depth;
  int nChannels = mouse->nChannels;

  printf("\n\n");
  printf("width: %d\n", width);
  printf("height: %d\n", height);

  IplImage *hsv = cvCreateImage(cvSize(width, height),depth,3);
  IplImage *hue = cvCreateImage(cvSize(width , height), depth, 1);
  IplImage *sat = cvCreateImage(cvSize(width , height), depth, 1);
  IplImage *img = cvCreateImage(cvSize(width , height), depth, 1);

  cvCvtColor(mouse, hsv, CV_RGB2HSV);
  cvSplit(hsv, hue, sat, img, (CvArr*) NULL);

  //get a rough region around the mouse using the first threshold
  double thresh1 = 70;
  IplImage *threshold = cvCreateImage(cvSize(img->width,img->height),img->depth,1);
  double max_value = 255;
  cvThreshold(img, threshold, thresh1, max_value, CV_THRESH_BINARY_INV);

  // clean up noise
  int cols = 3;
  int rows = 3;
  int anchor_x = 1;
  int anchor_y = 1;
  int shape = CV_SHAPE_RECT;
  IplConvKernel* kernel = cvCreateStructuringElementEx(cols, rows, anchor_x, anchor_y, shape, NULL );
  
  IplImage *erode = cvCreateImage(cvSize(width , height), depth, 1);
  int iterations = 2;
  cvErode(threshold, erode, kernel, iterations);
  
  // Enlarge the interest region
  cols = 4;
  rows = 4;
  kernel = cvCreateStructuringElementEx(cols, rows, anchor_x, anchor_y, shape, NULL );
  IplImage *dilate = cvCreateImage(cvSize(width , height), depth, 1);
  cvDilate(erode, dilate, kernel, 5);
  
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

  int i = 0;

  //Declare centroid and moments structure
  int Cx;
  int Cy;
  //nose and tail index
  CvPoint *nosePoint = NULL, *tailPoint = NULL;
  //int *nx, *ny, tx, ty;

  CvMoments moments;

  for ( ; contour != 0; contour = contour->h_next ) {
    i++;
    double contourArea = fabs( cvContourArea(contour, CV_WHOLE_SEQ) );
    
    printf("Contour Area %d: %g\n", i, contourArea);
    cvMoments(contour, &moments, 0);

    Cx = moments.m10/moments.m00;
    Cy = moments.m01/moments.m00;

    printf("centroid: (%d,%d) \n", Cx,Cy);

    cvCircle(mouse, cvPoint(Cx,Cy), 10, CV_RGB( 255, 0, 0 ), -1, 8, 0);

    //get Nose ( most distant point to the centroid )
    nosePoint = getFurthestPoint(contour, Cx, Cy);
    tailPoint = getFurthestPoint(contour, nosePoint->x, nosePoint->y);

    printf("nose (maybe): (%d,%d) \n", nosePoint->x, nosePoint->y);
    printf("tail (maybe): (%d,%d) \n", tailPoint->x, tailPoint->y);

    cvCircle(mouse, cvPoint(nosePoint->x, nosePoint->y), 5, CV_RGB( 0, 255, 0 ), -1, 8, 0);
    cvCircle(mouse, cvPoint(tailPoint->x, tailPoint->y), 5, CV_RGB( 0, 255, 0 ), -1, 8, 0);

    CvScalar externalColor = CV_RGB( 255, 255, 255 );
    CvScalar internalColor = CV_RGB( 0, 10, 0 );
    cvDrawContours(mouse, contour, externalColor, internalColor, 1, 2, 8 , offset);

  }

  // save image
  cvSaveImage("threshold.jpg", mouse);

  cvReleaseImage(&mouse);

  cvReleaseImage(&hsv);
  cvReleaseImage(&hue);
  cvReleaseImage(&sat);
  cvReleaseImage(&img);
  cvReleaseImage(&threshold);

  cvReleaseImage(&threshold);
  cvReleaseImage(&erode);
  cvReleaseImage(&dilate);
  cvReleaseImage(&contourImg);

}
