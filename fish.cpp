#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cv.h>
#include "highgui.h"

using namespace std;

int        g_slider_hue_max= 186;
int        g_slider_hue_min= 123;
int        g_slider_sat_max= 193;
int        g_slider_sat_min= 89;
int        g_slider_val_max= 249;
int        g_slider_val_min= 81;
CvScalar   g_hsv_max;
CvScalar   g_hsv_min;
CvCapture* g_capture         = NULL;

int     frames = 0;
void onTrackbarSlide(int pos) {
	g_hsv_max = cvScalar(g_slider_hue_max,
			     g_slider_sat_max,
			     g_slider_val_max
			);
	g_hsv_min = cvScalar(g_slider_hue_min,
			     g_slider_sat_min,
			     g_slider_val_min
			);
	/*cvSetCaptureProperty(
	  g_capture,
	  CV_CAP_PROP_POS_FRAMES,
	  pos
	  );*/
	//frames = pos;
	//g_slider_position = pos;
}

void set_trac_bar(){
	cvCreateTrackbar(
			"Hue Max",
			"set_HSV",
			&g_slider_hue_max,
			255,
			onTrackbarSlide
			);
	cvCreateTrackbar(
			"Hue Min",
			"set_HSV",
			&g_slider_hue_min,
			255,
			onTrackbarSlide
			);
	cvCreateTrackbar(
			"Sat Max",
			"set_HSV",
			&g_slider_sat_max,
			255,
			onTrackbarSlide
			);
	cvCreateTrackbar(
			"Sat Min",
			"set_HSV",
			&g_slider_sat_min,
			255,
			onTrackbarSlide
			);
	cvCreateTrackbar(
			"Val Max",
			"set_HSV",
			&g_slider_sat_max,
			255,
			onTrackbarSlide
			);
	cvCreateTrackbar(
			"Val Min",
			"set_HSV",
			&g_slider_val_min,
			255,
			onTrackbarSlide
			);
}

int main( int argc, char** argv ) {
	IplImage* frame;
	IplImage* gr_frame;
	CvSize    frameSize ;
	cvNamedWindow( "set_HSV", CV_WINDOW_NORMAL);
	cvNamedWindow( "Camera", CV_WINDOW_NORMAL);
	g_capture = cvCreateCameraCapture( 0 );
	frame = cvQueryFrame( g_capture );
	frameSize = cvGetSize(frame);
	gr_frame = cvCreateImage(frameSize,IPL_DEPTH_8U,1);
	set_trac_bar();
	onTrackbarSlide(0);

	//printf("hacked frames %d w %d h %d\n",frames,tmpw,tmph);


	frames = 0;
	while(1) {
		frame = cvQueryFrame( g_capture );
		cvSmooth( frame, frame, CV_GAUSSIAN, 3, 3 );
		cvShowImage( "Camera", frame );
		cvCvtColor(frame,frame,CV_RGB2HSV);
		cvInRangeS(frame,g_hsv_min,g_hsv_max,gr_frame);
		cvErode(gr_frame,gr_frame,NULL,4);
		cvDilate(gr_frame,gr_frame,NULL,4);
		cvShowImage( "set_HSV", gr_frame );
		char c = (char)cvWaitKey(10);
		if( c == 27 ) break;
	}
	cvReleaseCapture( &g_capture );
	cvDestroyWindow( "set_HSV" );
	cvDestroyWindow( "Camera" );
	return(0);
}
